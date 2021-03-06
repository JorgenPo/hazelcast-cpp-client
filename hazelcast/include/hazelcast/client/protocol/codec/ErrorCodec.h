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

/*
 * ErrorCodec.h
 *
 *  Created on: Apr 13, 2015
 *      Author: ihsan
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

#include "hazelcast/client/protocol/ResponseMessageConst.h"
#include "hazelcast/client/protocol/codec/StackTraceElement.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            class ClientMessage;

            namespace codec {
                class ErrorCodec {
                public:
                    int32_t errorCode;
                    std::string className;
                    std::unique_ptr<std::string> message;
                    std::vector<StackTraceElement> stackTrace;
                    int32_t causeErrorCode;
                    std::unique_ptr<std::string> causeClassName;

                    static const enum ResponseMessageConst TYPE = EXCEPTION;

                    /**
                    * Decode input byte array data into parameters
                    *
                    * @param message
                    * @return ErrorCodec
                    */
                    static ErrorCodec decode(ClientMessage &clientMessage);


                    std::string toString() const;

                    ErrorCodec(const ErrorCodec &rhs);
                private:
                    ErrorCodec(ClientMessage &clientMessage);
                };
            }
        }
    }
}


