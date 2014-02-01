/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#include <string>
#include <gtest/gtest.h>
#include <uavcan/internal/transport/transfer.hpp>


TEST(Transfer, TransferID)
{
    using uavcan::TransferID;

    // Tests below are based on this assumption
    ASSERT_EQ(16, 1 << TransferID::BITLEN);

    /*
     * forwardDistance()
     */
    EXPECT_EQ(0, TransferID(0).forwardDistance(0));
    EXPECT_EQ(1, TransferID(0).forwardDistance(1));
    EXPECT_EQ(15, TransferID(0).forwardDistance(15));

    EXPECT_EQ(0, TransferID(7).forwardDistance(7));
    EXPECT_EQ(15, TransferID(7).forwardDistance(6));
    EXPECT_EQ(1, TransferID(7).forwardDistance(8));

    EXPECT_EQ(9, TransferID(10).forwardDistance(3));
    EXPECT_EQ(7, TransferID(3).forwardDistance(10));

    EXPECT_EQ(8, TransferID(6).forwardDistance(14));
    EXPECT_EQ(8, TransferID(14).forwardDistance(6));

    EXPECT_EQ(1, TransferID(14).forwardDistance(15));
    EXPECT_EQ(2, TransferID(14).forwardDistance(0));
    EXPECT_EQ(4, TransferID(14).forwardDistance(2));

    EXPECT_EQ(15, TransferID(15).forwardDistance(14));
    EXPECT_EQ(14, TransferID(0).forwardDistance(14));
    EXPECT_EQ(12, TransferID(2).forwardDistance(14));

    /*
     * Misc
     */
    EXPECT_TRUE(TransferID(2) == TransferID(2));
    EXPECT_FALSE(TransferID(2) != TransferID(2));
    EXPECT_FALSE(TransferID(2) == TransferID(8));
    EXPECT_TRUE(TransferID(2) != TransferID(8));

    TransferID tid;
    for (int i = 0; i < 999; i++)
    {
        ASSERT_EQ(i & ((1 << TransferID::BITLEN) - 1), tid.get());
        const TransferID copy = tid;
        tid.increment();
        ASSERT_EQ(1, copy.forwardDistance(tid));
        ASSERT_EQ(15, tid.forwardDistance(copy));
        ASSERT_EQ(0, tid.forwardDistance(tid));
    }
}
