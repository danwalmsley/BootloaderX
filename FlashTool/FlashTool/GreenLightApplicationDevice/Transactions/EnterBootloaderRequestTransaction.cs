﻿namespace Gx.Transactions
{
    using IntegratedDebugProtocol.Transactions;
    using System.Collections.Generic;
    using IntegratedDebugProtocol;

    public class EnterBootloaderRequestTransaction : Transaction
    {
        public override ushort Id => 0xA000;

        public override bool Decode(List<byte> dataPayload)
        {
            return true;
        }

        public override bool Encode(IdpPacket packet)
        {
            return true;
        }
    }
}
