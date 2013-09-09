//
// Created by sancar koyunlu on 9/4/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "ClientQueueTest.h"
#include "HazelcastClient.h"
#include "HazelcastInstanceFactory.h"
#include "CountDownLatch.h"

namespace hazelcast {
    namespace client {
        namespace test {
            using namespace iTest;

            ClientQueueTest::ClientQueueTest(HazelcastInstanceFactory & hazelcastInstanceFactory)
            :hazelcastInstanceFactory(hazelcastInstanceFactory)
            , instance(hazelcastInstanceFactory)
            , client(new HazelcastClient(clientConfig.addAddress(Address("localhost", 5701))))
            , q(new IQueue< std::string>(client->getQueue< std::string >("clientQueueTest"))) {

            }

            ClientQueueTest::~ClientQueueTest() {
            }

            void ClientQueueTest::addTests() {
                addTest(&ClientQueueTest::testListener, "testListener");
                addTest(&ClientQueueTest::testOfferPoll, "testOfferPoll");
                addTest(&ClientQueueTest::testRemainingCapacity, "testRemainingCapacity");
                addTest(&ClientQueueTest::testRemove, "testRemove");
                addTest(&ClientQueueTest::testContains, "testContains");
                addTest(&ClientQueueTest::testDrain, "testDrain");
                addTest(&ClientQueueTest::testToArray, "testToArray");
                addTest(&ClientQueueTest::testAddAll, "testAddAll");
                addTest(&ClientQueueTest::testRemoveRetain, "testRemoveRetain");
                addTest(&ClientQueueTest::testClear, "testClear");
            }

            void ClientQueueTest::beforeClass() {

            }

            void ClientQueueTest::afterClass() {
                client.reset();
                instance.shutdown();
            }

            void ClientQueueTest::beforeTest() {
                q->clear();
            }

            void ClientQueueTest::afterTest() {
                q->clear();
            }

            class ItemListener {
            public:
                ItemListener(util::CountDownLatch& latch)
                :latch(latch) {

                }

                void itemAdded(impl::ItemEvent<std::string> itemEvent) {
                    latch.countDown();
                }

                void itemRemoved(impl::ItemEvent<std::string> item) {
                }

            private:
                util::CountDownLatch& latch;
            };


            void testListenerThread(IQueue<std::string> *q) {
                for (int i = 0; i < 5; i++) {
                    if (!q->offer(std::string("event_item") + util::to_string(i))) {
                        std::cerr << "error at testListenerThread" << std::endl;
                    }
                }
            }

            void ClientQueueTest::testListener() {
                assertEqual(0, q->size());

                util::CountDownLatch latch(5);

                ItemListener listener(latch);
                long id = q->addItemListener(listener, true);

                boost::this_thread::sleep(boost::posix_time::milliseconds(500));

                boost::thread t(boost::bind(testListenerThread, q.get()));

                assertTrue(latch.await(5 * 1000));
                q->removeItemListener(id);
            }

            void testOfferPollThread2(IQueue<std::string> *q) {
                boost::this_thread::sleep(boost::posix_time::seconds(2));
                q->offer("item1");
            }

            void ClientQueueTest::testOfferPoll() {
                for (int i = 0; i < 10; i++) {
                    bool result = q->offer("item");
                    assertTrue(result);
                }
                assertEqual(10, q->size());
                q->poll();
                bool result = q->offer("item", 5 * 1000);
                assertTrue(result);

                for (int i = 0; i < 10; i++) {
                    std::string o = q->poll();
                    assertTrue(o.compare("") != 0);
                }
                assertEqual(0, q->size());

                boost::thread t2(boost::bind(testOfferPollThread2, q.get()));

                std::string o = q->poll(5 * 1000);
                assertEqual("item1", o);
                t2.try_join_for(boost::chrono::seconds(10));
            }

            void ClientQueueTest::testRemainingCapacity() {
                int capacity = q->remainingCapacity();
                assertTrue(capacity > 10000);
                q->offer("item");
                assertEqual(capacity - 1, q->remainingCapacity());
            }


            void ClientQueueTest::testRemove() {
                assertTrue(q->offer("item1"));
                assertTrue(q->offer("item2"));
                assertTrue(q->offer("item3"));

                assertFalse(q->remove("item4"));
                assertEqual(3, q->size());

                assertTrue(q->remove("item2"));

                assertEqual(2, q->size());

                assertEqual("item1", q->poll());
                assertEqual("item3", q->poll());
            }


            void ClientQueueTest::testContains() {
                assertTrue(q->offer("item1"));
                assertTrue(q->offer("item2"));
                assertTrue(q->offer("item3"));
                assertTrue(q->offer("item4"));
                assertTrue(q->offer("item5"));


                assertTrue(q->contains("item3"));
                assertFalse(q->contains("item"));

                std::vector<std::string> list;
                list.push_back("item4");
                list.push_back("item2");

                assertTrue(q->containsAll(list));

                list.push_back("item");
                assertFalse(q->containsAll(list));

            }

            void ClientQueueTest::testDrain() {
                assertTrue(q->offer("item1"));
                assertTrue(q->offer("item2"));
                assertTrue(q->offer("item3"));
                assertTrue(q->offer("item4"));
                assertTrue(q->offer("item5"));

                std::vector<std::string> list;
                int result = q->drainTo(list, 2);
                assertEqual(2, result);
                assertEqual("item1", list[0]);
                assertEqual("item2", list[1]);

                std::vector<std::string> list2;
                result = q->drainTo(list2);
                assertEqual(3, result);
                assertEqual("item3", list2[0]);
                assertEqual("item4", list2[1]);
                assertEqual("item5", list2[2]);
            }

            void ClientQueueTest::testToArray() {
                assertTrue(q->offer("item1"));
                assertTrue(q->offer("item2"));
                assertTrue(q->offer("item3"));
                assertTrue(q->offer("item4"));
                assertTrue(q->offer("item5"));

                std::vector<string> array = q->toArray();
                for (int i = 0; i < array.size(); i++) {
                    assertEqual(std::string("item") + util::to_string(i + 1), array[i]);
                }

            }

            void ClientQueueTest::testAddAll() {
                std::vector<std::string > coll;
                coll.push_back("item1");
                coll.push_back("item2");
                coll.push_back("item3");
                coll.push_back("item4");

                assertTrue(q->addAll(coll));
                int size = q->size();
                assertEqual(size, coll.size());

            }

            void ClientQueueTest::testRemoveRetain() {
                assertTrue(q->offer("item1"));
                assertTrue(q->offer("item2"));
                assertTrue(q->offer("item3"));
                assertTrue(q->offer("item4"));
                assertTrue(q->offer("item5"));

                std::vector<std::string> list;
                list.push_back("item8");
                list.push_back("item9");
                assertFalse(q->removeAll(list));
                assertEqual(5, q->size());

                list.push_back("item3");
                list.push_back("item4");
                list.push_back("item1");
                assertTrue(q->removeAll(list));
                assertEqual(2, q->size());

                list.clear();
                list.push_back("item2");
                list.push_back("item5");
                assertFalse(q->retainAll(list));
                assertEqual(2, q->size());

                list.clear();
                assertTrue(q->retainAll(list));
                assertEqual(0, q->size());
            }

            void ClientQueueTest::testClear() {
                assertTrue(q->offer("item1"));
                assertTrue(q->offer("item2"));
                assertTrue(q->offer("item3"));
                assertTrue(q->offer("item4"));
                assertTrue(q->offer("item5"));

                q->clear();

                assertEqual(0, q->size());
                assertEqual("", q->poll());

            }
        }
    }
}