using System;
using System.Runtime.InteropServices;

public class AESOperations
{
    [DllImport("../../aes dll.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void encryptDecryptFile(
        [MarshalAs(UnmanagedType.LPStr)] string fileName,
        [MarshalAs(UnmanagedType.LPArray, SizeConst = 4), In] uint[,] key,
        [MarshalAs(UnmanagedType.LPArray, SizeConst = 4), In] uint[,] iv,
        uint isEncrypt);

    public void EncryptDecryptFile(string fileName, string key, string iv, uint isEncrypt)
    {
        uint[,] key2D = new uint[4, 4];
        uint[,] iv2D = new uint[4, 4];

        StrTo2D(key, key2D);
        StrTo2D(iv, iv2D);

        encryptDecryptFile(fileName, key2D, iv2D, isEncrypt);
    }

    public void StrTo2D(string password, uint[,] arr)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                arr[i,j] = password[i * 4 + j];
            }
        }
    }
}
