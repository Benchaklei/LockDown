using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using Newtonsoft.Json;

namespace LockDownClient
{
    class Communicator
    {
        public static class Constants
        {
            public const int signup_code = 101;
            public const int login_code = 102;
            public const int start_encrypt_code = 103;
            public const int start_decrypt_code = 104;
            public const int logout_code = 105;

            public const string signup_success = "201";
            public const string login_success = "202";
            public const string start_encrypt_success = "203";
            public const string start_decrypt_success = "204";
            public const string logout_success = "205";


            public const int code_len = 1;
            public const int data_len = 4;
        }

        public static string turn_to_binary(int length, int var) // the function turns the variable into bytes
        {
            string var_byte = char.ToString((char)var); // the variable in bytes

            if (var_byte.Length < length)
            {
                while (var_byte.Length < length)
                {
                    var_byte = char.ToString((char)0) + var_byte;
                }
            }
            return var_byte;
        }

        public static string SendMessage(Socket socketToSend, string data, int msgType)
        {
            string msg = msgType + data;

            socketToSend.Send(Encoding.ASCII.GetBytes(msg));

            byte[] reciveBuffer = new byte[1024];

            //2. receive the data from the server into the byte array
            int rec = socketToSend.Receive(reciveBuffer);

            //3. prepare byte array with the size of bytes how we receive from the server
            byte[] dataArr = new byte[rec];

            //4. copy the byte array how reive in to the byte array with the correct size
            Array.Copy(reciveBuffer, dataArr, rec);

            //5. convert the byte array to Ascii and return
            return Encoding.ASCII.GetString(dataArr);
        }

        public static string SendLogOrSignUpReq(Socket socketToSend, string name, string password, bool isLogin) // this function will send to the server the login/signup request
        {
            IDictionary<string, string> data = new Dictionary<string, string>();

            int msgType = Constants.login_code;

            data.Add("username", name);
            data.Add("password", password);

            if (!isLogin)
            {
                msgType = Constants.signup_code;
            }

            try
            {
                return SendMessage(socketToSend, JsonConvert.SerializeObject(data), msgType);
            }
            catch
            {
                return "";
            }
        }

        public static string SendEncryptionRequest(Socket socketToSend) // this function will send an encryption request to the server
        {
            int msgType = Constants.start_encrypt_code;

            try
            {
                return SendMessage(socketToSend, "", msgType);
            }
            catch
            {
                return "";
            }
        }

        public static string SendDecryptionRequest(Socket socketToSend) // this function will send a decryption request to the server
        {
            IDictionary<string, string> data = new Dictionary<string, string>();

            int msgType = Constants.start_decrypt_code;

            try
            {
                return SendMessage(socketToSend, JsonConvert.SerializeObject(data), msgType);
            }
            catch
            {
                return "";
            }
        }
    }
}
