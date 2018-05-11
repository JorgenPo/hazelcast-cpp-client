/*
 * Copyright (c) 2008-2018, Hazelcast, Inc. All Rights Reserved.
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

#include "hazelcast/util/Util.h"
#include "hazelcast/util/ILogger.h"

#include "hazelcast/client/protocol/codec/QueueCompareAndRetainAllCodec.h"
#include "hazelcast/client/serialization/pimpl/Data.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace codec {
                const QueueMessageType QueueCompareAndRetainAllCodec::REQUEST_TYPE = HZ_QUEUE_COMPAREANDRETAINALL;
                const bool QueueCompareAndRetainAllCodec::RETRYABLE = false;
                const ResponseMessageConst QueueCompareAndRetainAllCodec::RESPONSE_TYPE = (ResponseMessageConst) 101;

                std::auto_ptr<ClientMessage> QueueCompareAndRetainAllCodec::encodeRequest(
                        const std::string &name,
                        const std::vector<serialization::pimpl::Data> &dataList) {
                    int32_t requiredDataSize = calculateDataSize(name, dataList);
                    std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                    clientMessage->setMessageType((uint16_t) QueueCompareAndRetainAllCodec::REQUEST_TYPE);
                    clientMessage->setRetryable(RETRYABLE);
                    clientMessage->set(name);
                    clientMessage->setArray<serialization::pimpl::Data>(dataList);
                    clientMessage->updateFrameLength();
                    return clientMessage;
                }

                int32_t QueueCompareAndRetainAllCodec::calculateDataSize(
                        const std::string &name,
                        const std::vector<serialization::pimpl::Data> &dataList) {
                    int32_t dataSize = ClientMessage::HEADER_SIZE;
                    dataSize += ClientMessage::calculateDataSize(name);
                    dataSize += ClientMessage::calculateDataSize<serialization::pimpl::Data>(dataList);
                    return dataSize;
                }

                QueueCompareAndRetainAllCodec::ResponseParameters::ResponseParameters(ClientMessage &clientMessage) {


                    response = clientMessage.get<bool>();

                }

                QueueCompareAndRetainAllCodec::ResponseParameters
                QueueCompareAndRetainAllCodec::ResponseParameters::decode(ClientMessage &clientMessage) {
                    return QueueCompareAndRetainAllCodec::ResponseParameters(clientMessage);
                }


            }
        }
    }
}

