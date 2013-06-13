//
// Created by sancar koyunlu on 6/11/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST__MAP_REQUEST_ID
#define HAZELCAST__MAP_REQUEST_ID

namespace hazelcast {
    namespace client {
        namespace map {
            namespace RequestIDs {
                enum {
                    F_ID = -10,
                    GET = 1,
                    PUT = 2,
                    PUT_IF_ABSENT = 3,
                    TRY_PUT = 4,
                    PUT_TRANSIENT = 5,
                    SET = 6,
                    CONTAINS_KEY = 7,
                    CONTAINS_VALUE = 8,
                    REMOVE = 9,
                    REMOVE_IF_SAME = 10,
                    DELETE = 11,
                    FLUSH = 12,
                    GET_ALL = 13,
                    TRY_REMOVE = 14,
                    REPLACE = 15,
                    REPLACE_IF_SAME = 16,
                    LOCK = 17,
                    IS_LOCKED = 18,
                    UNLOCK = 20,
                    EVICT = 21,
                    ADD_INTERCEPTOR = 23,
                    REMOVE_INTERCEPTOR = 24,
                    ADD_ENTRY_LISTENER = 25,
                    GET_ENTRY_VIEW = 27,
                    ADD_INDEX = 28,
                    KEY_SET = 29,
                    VALUES = 30,
                    ENTRY_SET = 31,
                    SIZE = 33,
                    QUERY = 34,
                    CLEAR = 35,
                    GET_LOCAL_MAP_STATS = 37,
                    EXECUTE_ON_KEY = 38,
                    EXECUTE_ON_ALL_KEYS = 39,
                    PUT_ALL = 40,
                    DESTROY = 41
                };
            }

        }
    }
}


#endif //HAZELCAST__MAP_REQUEST_ID