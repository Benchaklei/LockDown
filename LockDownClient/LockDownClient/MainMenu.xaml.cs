using System.Net.Sockets;
using System.Windows;
using Microsoft.Win32;
using System.Windows.Input;
using System.Windows.Controls;
using System;

namespace LockDownClient
{
    public partial class MainMenu : Window
    {
        private static Socket _clientSocket;
        private static AESOperations op;


        public MainMenu(Socket sock)
        {
            InitializeComponent();
            _clientSocket = sock;
            op = new AESOperations();
        }

        private void EncOrDecButton(object sender, RoutedEventArgs e)
        {
            if (_clientSocket.Connected)
            {
                const int keyAndIvSize = 16;
                Button caller = (Button)sender;
                string answer = "";
                uint isEnc = (uint)Convert.ToInt32(caller.Content.Equals("Encrypt"));
                if (isEnc == 1)
                {
                    answer = Communicator.SendEncryptionRequest(_clientSocket);
                }
                else
                {
                    answer = Communicator.SendDecryptionRequest(_clientSocket);
                }

                string data = answer.Substring(3);

                if (answer.Contains(Communicator.Constants.start_encrypt_success) || answer.Contains(Communicator.Constants.start_decrypt_success))
                {
                    string key = data.Substring(0, keyAndIvSize);
                    string iv = data.Substring(keyAndIvSize);

                    EncryptDecryptFunc(key, iv, isEnc);

                    errMsg.Text = (isEnc == 1 ? "Encryption" : "Decryption") + " Succeeded!";
                    MessageBox.Show(data);
                }
                else
                {
                    errMsg.Text = "Encryption  Failed!";
                }
            }
            else
            {
                errMsg.Text = "Can't connect to server!";
            }
        }

        private void Image_MouseUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void Border_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                DragMove();
        }

        private string getFilePath()
        {
            OpenFileDialog fileChoiceWindow = new OpenFileDialog();
            if (fileChoiceWindow.ShowDialog() == true)
            {
                return fileChoiceWindow.FileName;
            }
            return "Failed Opening File!";
        }
        private void EncryptDecryptFunc(string key, string iv, uint isEnc)
        {
            string pathToEnc = getFilePath();
            op.EncryptDecryptFile(pathToEnc, key, iv, isEnc);
        }
    }
}