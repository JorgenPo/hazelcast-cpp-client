//
//  ClassDefinitionWriter.h
//  Server
//
//  Created by sancar koyunlu on 1/10/13.
//  Copyright (c) 2013 sancar koyunlu. All rights reserved.
//

#ifndef HAZELCAST_CLASS_DEFINITION_WRITER
#define HAZELCAST_CLASS_DEFINITION_WRITER

#include "FieldDefinition.h"
#include "HazelcastException.h"
#include "FieldType.h"
#include "ConstantSerializers.h"
#include "ClassDefinition.h"
#include "Portable.h"
#include "SerializationContext.h"
#include "boost/type_traits/is_base_of.hpp"
#include <boost/shared_ptr.hpp>
#include <string>
#include <iosfwd>

using namespace std;

namespace hazelcast {
    namespace client {
        namespace serialization {

            class ClassDefinitionWriter {
            public:

                ClassDefinitionWriter(int factoryId, int classId, int version, SerializationContext *serializationContext);

                ClassDefinitionWriter& operator [](std::string fieldName);

                void writeInt(int value);

                void writeLong(long value);

                void writeBoolean(bool value);

                void writeByte(byte value);

                void writeChar(int value);

                void writeDouble(double value);

                void writeFloat(float value);

                void writeShort(short value);

                void writeUTF(const string& str);

                void writeNullPortable(int factoryId, int classId);

                void writeByteArray(const std::vector<byte>&);

                void writeCharArray(const std::vector<char>&);

                void writeIntArray(const std::vector<int>&);

                void writeLongArray(const std::vector<long>&);

                void writeDoubleArray(const std::vector<double>&);

                void writeFloatArray(const std::vector<float>&);

                void writeShortArray(const std::vector<short>&);

                template <typename T>
                void writePortable(T& portable) {
                    if (writingPortable) {
                        FieldDefinition fd(index++, lastFieldName, FieldTypes::TYPE_PORTABLE, getFactoryId(portable), getClassId(portable));
                        addNestedField(portable, fd);
                        writingPortable = false;
                    } else {
                        throw hazelcast::client::HazelcastException("Can not write portable directly(without using ['fieldName'])");
                    }
                };

                template <typename T>
                void writePortable(const std::vector<T>& portables) {
                    if (writingPortable) {
                        int classId = getClassId(portables[0]);
                        int factoryId = getFactoryId(portables[0]);
                        FieldDefinition fd(index++, lastFieldName, FieldTypes::TYPE_PORTABLE_ARRAY, factoryId, classId);
                        addNestedField(portables[0], fd);
                    } else {
                        throw hazelcast::client::HazelcastException("Can not write portable directly(without using ['fieldName'])");
                    }
                };

                boost::shared_ptr<ClassDefinition> getClassDefinition();

            private:
                void addField(FieldType const&);

                template <typename T>
                void addNestedField(T& p, FieldDefinition& fd) {
                    cd->add(fd);
                    boost::shared_ptr<ClassDefinition> nestedCd = getOrBuildClassDefinition(p);
                    cd->add(nestedCd);
                };

                template <typename T>
                boost::shared_ptr<ClassDefinition> getOrBuildClassDefinition(const T& p) {
                    boost::shared_ptr<ClassDefinition> cd;

                    int factoryId = getFactoryId(p);
                    int classId = getClassId(p);
                    if (context->isClassDefinitionExists(factoryId, classId)) {
                        cd = context->lookup(factoryId, classId);
                    } else {
                        ClassDefinitionWriter classDefinitionWriter(factoryId, classId, context->getVersion(), context);
                        hazelcast::client::serialization::writePortable(classDefinitionWriter, p);
                        cd = classDefinitionWriter.getClassDefinition();
                        context->registerClassDefinition(cd);
                    }

                    return cd;
                };

                int factoryId;
                int classId;
                int index;
                bool raw;
                bool writingPortable;
                std::string lastFieldName;
                boost::shared_ptr<ClassDefinition> cd;
                SerializationContext *context;

            };

            template<typename T>
            inline void writePortable(ClassDefinitionWriter& classDefinitionWriter, const std::vector<T>& data) {
                classDefinitionWriter.writePortable(data);
            };

            template<typename T>
            inline void operator <<(ClassDefinitionWriter& classDefinitionWriter, T& data) {
                if (boost::is_base_of<Portable, T>::value)
                    classDefinitionWriter.writePortable(data);
                else
                    writePortable(classDefinitionWriter, data);
            };

        }
    }
}
#endif /* HAZELCAST_CLASS_DEFINITION_WRITER */
