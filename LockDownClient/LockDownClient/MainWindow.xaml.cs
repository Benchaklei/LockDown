using System.Net;
using System.Net.Sockets;
using System.Windows;
using System.Windows.Input;

namespace LockDownClient
{
    public partial class MainWindow : Window
    {
        private static Socket _clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        private static AESOperations op;

        public MainWindow()
        {
            InitializeComponent();
            op = new AESOperations();

            ConnectToServer();
        }

        public MainWindow(Socket sock)
        {
            InitializeComponent();

            _clientSocket = sock;
        }

        private void ConnectToServer() /// This function will connect the client to the server
        {
            if (!_clientSocket.Connected)
            {
                try
                {
                    _clientSocket.Connect(IPAddress.Loopback, 42069);
                    errMsg.Text = "";
                }
                catch (SocketException)
                {
                    // SET THE ERROR TXT TO "CANNOT CONNECT TO SERVER"
                    errMsg.Text = "Can't connect to server!";
                }
            }
        }

        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                DragMove();
        }

        private void Image_MouseUp(object sender, MouseButtonEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void PasswordBox_PasswordChanged(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrEmpty(passwordBox.Password) && passwordBox.Password.Length > 0)
                textPassword.Visibility = Visibility.Collapsed;
            else
                textPassword.Visibility = Visibility.Visible;
        }

        private void textPassword_MouseDown(object sender, MouseButtonEventArgs e)
        {
            passwordBox.Focus();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

            if (_clientSocket.Connected)
            {
                string name = txtEmail.Text;
                string pass = passwordBox.Password;
                string answer = Communicator.SendLogOrSignUpReq(_clientSocket, name, pass, true);

                if (answer.Contains(Communicator.Constants.login_success))
                {
                    MainMenu w = new MainMenu(_clientSocket);
                    w.Show();
                    this.Visibility = Visibility.Hidden;
                }
                else if (answer.Contains("already"))
                {
                    errMsg.Text = "User is already logged!";
                }
                else
                {
                    errMsg.Text = "Login Failed!";
                }
            }
            else
            {
                errMsg.Text = "Can't connect to server!";
            }
        }

        private void txtEmail_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        {
            if (!string.IsNullOrEmpty(txtEmail.Text) && txtEmail.Text.Length > 0)
                textEmail.Visibility = Visibility.Collapsed;
            else
                textEmail.Visibility = Visibility.Visible;
        }

        private void textEmail_MouseDown(object sender, MouseButtonEventArgs e)
        {
            txtEmail.Focus();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            Registration r = new Registration(this, _clientSocket);
            r.Top = this.Top;
            r.Left = this.Left;
            r.Show();

            this.Visibility = Visibility.Hidden;
        }
    }
}
