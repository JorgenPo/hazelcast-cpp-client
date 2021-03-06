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
#include <hazelcast/client/EntryListener.h>
#include <hazelcast/client/HazelcastClient.h>

int main() {
    hazelcast::client::ClientConfig config;

    /**
     * Number of the threads to handle the incoming event packets.
     * The default is 5.
     *
     * We force the clisnt use only a single thread for events in this example.
     */
    config.setProperty("hazelcast.client.event.thread.count", "1");

    /**
     * Capacity of the executor that handles the incoming event packets.
     *
     * The default value is 1000000.
     *
     * This example sets the max capacity to 50000.
     */
    config.setProperty("hazelcast.client.event.queue.capacity", "50000");

    hazelcast::client::HazelcastClient hz(config);

    auto map = hz.getMap("MyMap");

    hazelcast::client::EntryListener listener;

    listener.
        onEntryAdded([](const hazelcast::client::EntryEvent &event) {
            std::cout << "Entry added:" << event.getKey().get<int>().value();
        }).
        onEntryRemoved([](const hazelcast::client::EntryEvent &event) {
            std::cout << "Entry removed:" << event.getKey().get<int>().value();
        }).
        onEntryUpdated([](const hazelcast::client::EntryEvent &event) {
            std::cout << "Entry updated:" << event.getKey().get<int>().value();
        }).
        onEntryEvicted([](const hazelcast::client::EntryEvent &event) {
            std::cout << "Entry evicted:" << event.getKey().get<int>().value();
        }).
        onEntryExpired([](const hazelcast::client::EntryEvent &event) {
            std::cout << "Entry expired:" << event.getKey().get<int>().value();
        }).
        onEntryMerged([](const hazelcast::client::EntryEvent &event) {
            std::cout << "Entry merged:" << event.getKey().get<int>().value();
        }).
        onMapEvicted([](const hazelcast::client::MapEvent &event) {
            std::cout << "Map evicted:" << event.getName();
        }).
        onMapCleared([](const hazelcast::client::MapEvent &event) {
            std::cout << "Map cleared:" << event.getName();
        });

    map->addEntryListener(std::move(listener), false).get();

    // Now we put two entries, and since there is only one event thread, they will be delivered to the entry listener,
    // from within the same thread, hence it will be a sequential delivery. Hence we should see that "Entry added:100"
    // is printed before "Entry added:200"
    map->put(1, 100).get();
    map->put(2, 200).get();

    std::cout << "Finished" << std::endl;

    hz.shutdown();

    return 0;
}
