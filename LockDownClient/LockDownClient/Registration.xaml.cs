using System.Net.Sockets;
using System.Windows;
using System.Windows.Input;

namespace LockDownClient
{
    public partial class Registration : Window
    {
        private static Socket _clientSocket;
        private static MainWindow _mainWindow;

        public Registration(MainWindow mw, Socket sock)
        {
            InitializeComponent();
            _clientSocket = sock;
            _mainWindow = mw;
        }

        private void SignUpFunc(object sender, RoutedEventArgs e)
        {
            if (_clientSocket.Connected)
            {
                string name = txtEmail.Text;
                string pass = passwordBox.Password;
                string answer = Communicator.SendLogOrSignUpReq(_clientSocket, name, pass, false);

                if (answer.Contains(Communicator.Constants.signup_success))
                {
                    errMsg.Text = "Thanks for signing up!";
                }
                else
                {
                    errMsg.Text = "Signup failed!";
                }
            }
            else
            {
                errMsg.Text = "Can't connect to server!";
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
            this.Visibility = Visibility.Hidden;
            _mainWindow.Top = this.Top;
            _mainWindow.Left = this.Left;
            _mainWindow.Show();
        }
    }
}
