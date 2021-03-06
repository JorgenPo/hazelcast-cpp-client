/*
 * Copyright (c) 2008-2020, Hazelcast, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hazelcast/client/HazelcastClient.h>

struct Value {
    int32_t amount;
    int32_t version;
};

namespace hazelcast {
    namespace client {
        namespace serialization {
            template<>
            struct hz_serializer<Value> : identified_data_serializer {
                static int32_t getFactoryId() noexcept {
                    return 1;
                }

                static int32_t getClassId() noexcept {
                    return 6;
                }

                static void writeData(const Value &object, hazelcast::client::serialization::ObjectDataOutput &out) {
                    out.write(object.amount);
                    out.write(object.version);
                }

                static Value readData(hazelcast::client::serialization::ObjectDataInput &in) {
                    return Value{in.read<int32_t>(), in.read<int32_t>()};
                }
            };
        }
    }
}

int main() {
    hazelcast::client::HazelcastClient hz;

    auto map = hz.getMap("map");

    std::string key("1");
    Value v;
    map->put(key, v).get();
    std::cout << "Starting" << std::endl;
    for (int k = 0; k < 1000; k++) {
        map->lock(key);
        try {
            map->get<std::string, Value>(key).get();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            v.amount++;
            map->put(key, v).get();

            map->unlock(key).get();
        } catch (hazelcast::client::exception::IException &e) {
            map->unlock(key).get();
            throw e;
        }
    }
    std::cout << "Finished! Result = " << map->get<std::string, Value>(key).get()->amount << std::endl;

    std::cout << "Finished" << std::endl;

    return 0;
}
