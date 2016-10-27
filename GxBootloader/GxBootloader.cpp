/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "CRC.h"
#include "Comms/CommsHandlers.h"
#include "GxBootloader.h"
#include "Kernel.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
GxBootloader::GxBootloader (IBoard& board, uint32_t encryptionKey)
    : board (board), UsbInterface (GxBootloaderHidDevice (*board.hidDevice))
{
    this->encryptionKey = encryptionKey;
    Dispatcher::GetMainDispatcher ().BeginInvoke (Action::Create<GxBootloader, &GxBootloader::Initialise> (this));

    /*UsbInterface.BootloaderVersionRequested +=
    [&](void* sender, EventArgs& args)
    {
        UsbInterface.SendBootloaderVersion (GxBootloader::Version);
    };

    UsbInterface.EraseFirmwareRequested += [&](void* sender, EventArgs& args)
    {
        board.BootloaderService->EraseFirmware ();

        currentAddress = 0x0800C000;
        UsbInterface.Acknowlege (0);
    };

    UsbInterface.FlashDataRequested += [&](void* sender, EventArgs& args)
    {
        auto flashArgs = (FlashDataEventArgs&)args;

        for (uint8_t i = 0; i < flashArgs.length;)
        {
            uint8_t remainingSize = flashArgs.length - i;

            if (remainingSize >= 4)
            {
                uint32_t* data = (uint32_t*)&flashArgs.data[i];
                board.BootloaderService->FlashData (currentAddress, *data);
                i += 4;
                currentAddress += 4;
            }
            else if (remainingSize >= 2)
            {
                uint16_t* data = (uint16_t*)&flashArgs.data[i];
                board.BootloaderService->FlashData (currentAddress, *data);
                i += 2;
                currentAddress += 2;
            }
            else if (remainingSize >= 1)
            {
                board.BootloaderService->FlashData (currentAddress,
                                                    flashArgs.data[i]);
                i++;
                currentAddress++;
            }
        }

        UsbInterface.Acknowlege (1);
    };

    UsbInterface.FinaliseImageRequested += [&](void* sender, EventArgs& args)
    {
        SetState (BootloaderState::Normal);

        UsbInterface.Acknowlege (1);

        board.BootloaderService->SystemReset ();
    };*/
}

GxBootloader::~GxBootloader ()
{
}

void GxBootloader::InitialiseFlags ()
{
    BootloaderFlags flags;

    flags.Version = GxBootloader::Version;

    board.BootloaderService->WriteFlags (&flags);
}

void GxBootloader::SetState (BootloaderState state)
{
    auto flags = *board.BootloaderService->ReadFlags ();

    flags.State = state;

    board.BootloaderService->WriteFlags (&flags);
}

void GxBootloader::Run ()
{
}

void GxBootloader::Initialise ()
{
    if (!board.BootloaderService->ReadFlags ()->IsBootloaderPresent ())
    {
        InitialiseFlags ();
    }

    if (board.BootloaderService->ReadFlags ()->Version != Version)
    {
        auto flags = *board.BootloaderService->ReadFlags ();

        flags.Version = Version;

        board.BootloaderService->WriteFlags (&flags);
    }

    if (board.ForceBootloadRequested ())
    {
        SetState (BootloaderState::Bootloader);
    }
    else if (board.BootloaderService->ReadFlags ()->State == BootloaderState::Normal)
    {
        board.BootloaderService->JumpToApplication ();

        SetState (BootloaderState::Bootloader);
    }

    board.hidDevice->InitialiseStack ();

    new CommsHandlers (*this, *board.BootloaderService);
}

uint32_t GxBootloader::EncryptDecrypt (uint32_t key, uint32_t& scrambleKey, uint32_t data)
{
    uint8_t* scrambleBytes = reinterpret_cast<uint8_t*> (&scrambleKey);

    uint16_t crc = static_cast<uint16_t> (scrambleKey);

    for (int i = 0; i < sizeof (uint32_t); i++)
    {
        crc = CRC::Crc16 (crc, *scrambleBytes++);
    }

    scrambleKey = scrambleKey & 0xFFFF0000;
    scrambleKey |= crc;

    scrambleBytes = reinterpret_cast<uint8_t*> (&scrambleKey);

    crc = static_cast<uint16_t> (scrambleKey);

    for (int i = 0; i < sizeof (uint32_t); i++)
    {
        crc = CRC::Crc16 (crc, *scrambleBytes++);
    }

    scrambleKey = scrambleKey & 0x0000FFFF;
    scrambleKey |= ((uint32_t)crc) << 16;

    return data ^ key ^ scrambleKey;
}
