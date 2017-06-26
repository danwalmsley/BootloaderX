﻿namespace Gx.Transactions
{
    using IntegratedDebugProtocol.Transactions;
    using System.Collections.Generic;
    using IntegratedDebugProtocol;

    public class BeginFlashOperationRequestTransaction : Transaction
    {
        public override ushort Id => 0xB004;

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