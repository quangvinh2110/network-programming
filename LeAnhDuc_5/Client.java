package socket;

// 21020761 Lê Anh Đức
import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) {
        int PORT = 8080;
        Socket socket = null;
        //đặt ngoại lệ để kết nối với server
        try {
            socket = new Socket("127.0.0.1", PORT);
            System.out.println("Connected to the server...");

            DataInputStream in = new DataInputStream(socket.getInputStream());
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());

            String buff;
            int n;

            // gửi tin nhắn cho Server
            for (;;) {
                buff = "";
                System.out.print("\nEnter the string : ");
                n = 0;

                // nhập tin nhăắn và gửi đi
                while ((n = System.in.read()) != -1) {
                    buff += (char) n;
                    if (n == '\n') break;
                }
                out.writeUTF(buff);
                // nhận phản hồi từ server
                buff = in.readUTF();
                System.out.println("\nFrom Server : " + buff);

                if (buff.startsWith("exit")) {
                    System.out.println("Client Exit...");
                    break;
                }
            }
            // đóng kết nối
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (socket != null) {
                    socket.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
