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

#pragma once

#if  defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(push)
#pragma warning(disable: 4251) //for dll export
#endif

#include <string>
#include <stdint.h>
#include <memory>

#include "hazelcast/util/HazelcastDll.h"

namespace hazelcast {
    namespace client {
        namespace connection {
            class Connection;
        }
        namespace spi {
            namespace impl {
                class ListenerMessageCodec;

                namespace listener {
                    /**
                     * Keeps the information related to to an event registration made by clients.
                     */
                    class HAZELCAST_API ClientEventRegistration {
                    public:
                        ClientEventRegistration();

                        ClientEventRegistration(const std::string &serverRegistrationId, int64_t callId,
                                                const std::shared_ptr<connection::Connection> &subscriber,
                                                const std::shared_ptr<ListenerMessageCodec> &codec);

                        /**
                         * Alias registration ID is same as registration ID in the beginning. If listener had to be re-registered
                         * new registration ID is stored as server registration ID.
                         * When user try to remove the listener with registration ID, related server registration is send to
                         * subscribed member to remove the listener.
                         *
                         * @return server registration ID
                         */
                        const std::string &getServerRegistrationId() const;

                        /**
                         * Call ID of first event registration request
                         *
                         * @return call ID
                         */
                        int64_t getCallId() const;

                        /**
                         * This is used when removing the listener.
                         * Note: Listeners need to be removed from the member that they are first subscribed.
                         *
                         * @return subscriber
                         */
                        const std::shared_ptr<connection::Connection> &getSubscriber() const;

                        const std::shared_ptr<ListenerMessageCodec> &getCodec() const;

                        bool operator==(const ClientEventRegistration &rhs) const;

                        bool operator!=(const ClientEventRegistration &rhs) const;

                        bool operator<(const ClientEventRegistration &rhs) const;

                    private:
                        std::string serverRegistrationId;
                        int64_t callId;
                        std::shared_ptr<connection::Connection> subscriber;
                        std::shared_ptr<ListenerMessageCodec> codec;
                    };
                }
            }
        }
    }
}

#if  defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(pop)
#endif


