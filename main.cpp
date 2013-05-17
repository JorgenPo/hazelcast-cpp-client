//
// server.cpp
#include "TestMobile.h"
#include "portableTest.h"

using namespace hazelcast::client::serialization;

void testNewDesign();

int main(int argc, char **argv) {
    try{
        testDifferentVersions();
        testRawData();
        testRawDataWithoutRegistering();
        testRawDataInvalidWrite();
        testRawDataInvalidRead();
        testCompression();
        testSerialization();
        testSerializationViaFile();
        testNewDesign();

//        testBinaryClient();
//        testMapOperations();
//        testMapLocksInSequential();
//        testMapLocksInParallel();
//        testSpeed();
//        testPut();


        std::cout << "Test are completed successfully" << std::endl;
//        std::cin >> argc;

    } catch (hazelcast::client::HazelcastException exception) {
        std::cout << exception.what() << std::endl;
    } catch(void *s){
        std::cout << s << std::endl;
    }
    return 0;
};


void testNewDesign() {
    SerializationService serializationService(1);
    Data data;
    TestMobile np(4, 'k');
    data = serializationService.toData(np);

    TestMobile tnp1;
    tnp1 = serializationService.toObject<TestMobile>(data);

    assert(np == tnp1);
    int x = 4;
    data = serializationService.toData(x);
    int y = serializationService.toObject<int>(data);
    assert(x == y);
};