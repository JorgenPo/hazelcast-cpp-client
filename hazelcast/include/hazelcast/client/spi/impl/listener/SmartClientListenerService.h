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

#include "hazelcast/client/spi/impl/listener/AbstractClientListenerService.h"
#include "hazelcast/client/spi/EventHandler.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            class ClientMessage;
        }

        namespace spi {
            namespace impl {
                namespace listener {
                    class SmartClientListenerService : public AbstractClientListenerService {
                    public:
                        SmartClientListenerService(ClientContext &clientContext, int32_t eventThreadCount);

                        void start() override;

                        void shutdown() override;

                        boost::future<std::string>
                        registerListener(std::unique_ptr<impl::ListenerMessageCodec> &&listenerMessageCodec,
                                         std::unique_ptr<client::impl::BaseEventHandler> &&handler) override;

                        void asyncConnectToAllMembersInternal();

                    private:
                        std::shared_ptr<boost::asio::steady_timer> timer;

                        bool registersLocalOnly() const override;

                        void trySyncConnectToAllMembers();

                        void timeOutOrSleepBeforeNextTry(std::chrono::steady_clock::time_point start, const Member &lastFailedMember,
                                                         std::exception_ptr lastException);

                        void
                        throwOperationTimeoutException(std::chrono::steady_clock::time_point start,
                                                       std::chrono::steady_clock::time_point now,
                                                       std::chrono::steady_clock::duration elapsed,
                                                       const Member &lastFailedMember,
                                                       std::exception_ptr lastException);

                        void sleepBeforeNextTry();

                        void scheduleConnectToAllMembers();
                    };
                }
            }
        }
    }
}


