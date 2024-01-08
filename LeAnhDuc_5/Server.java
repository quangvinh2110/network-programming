package socket;
// 21020761 Lê Anh Đức
import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        int PORT = 8080;
        ServerSocket serverSocket = null;
        Socket clientSocket = null;
        // đặt ngoại lệ tạo kết nối với client
        try {
            serverSocket = new ServerSocket(PORT);
            System.out.println("Server listening...");

            clientSocket = serverSocket.accept();
            System.out.println("Server accepted the client...");

            DataInputStream in = new DataInputStream(clientSocket.getInputStream());
            DataOutputStream out = new DataOutputStream(clientSocket.getOutputStream());

            String buff;
            int check = 0;
            int checkName = 0;
            int checkAge = 0;
            int userValid = 0;
            int ageValid = 0;
            int i;
            int send = 0;
            // Vòng lặp vô hạn để giao tiếp với client
            while (true) {
                buff = in.readUTF();
                System.out.println("From client: " + buff + "\t To client: ");
                // kiểm tra xem tên có đúng không
                if (check == 1) {
                    if (checkName == 1) {
                        int valid = 1;
                        for (i = 0; i < buff.length() - 1; i++) {
                            if (!Character.isAlphabetic(buff.charAt(i))) {
                                valid = 0;
                                break;
                            }
                        }
                        if (valid == 1) {
                            out.writeUTF("211 User name OK " + buff);
                            ageValid = 1;
                            checkName = 0;
                            check = 0;
                            send = 1;
                        } else {
                            out.writeUTF("sai o day");
                            out.writeUTF("410 User name error ");
                            send = 1;
                        }
                    }
                    // kiểm tra xem tuổi có đúng không
                    if (checkAge == 1) {
                        int valid = 1;
                        for (i = 0; i < buff.length() - 1; i++) {
                            if (buff.charAt(i) > '9' || buff.charAt(i) < '0') {
                                valid = 0;
                                break;
                            }
                        }
                        if (valid == 1) {
                            out.writeUTF("221 User age OK " + buff);
                            ageValid = 1;
                            checkAge = 0;
                            check = 0;
                            send = 1;
                        } else {
                            out.writeUTF("420 User age error");
                            send = 1;
                        }
                    }
                }
                // nếu client nhập vào 1 trong những yêu cầu sau thì server gửi luôn tin nhắn về
                if (buff.startsWith("HELLO")) {
                    out.writeUTF("200 Hello Client");
                    send = 1;
                }
                if (buff.startsWith("USER NAME")) {
                    out.writeUTF("210 User name OK");
                    check = 1;
                    checkName = 1;
                    send = 1;
                }
                if (buff.startsWith("USER AGE")) {
                    out.writeUTF("220 User age OK");
                    check = 1;
                    checkAge = 1;
                    send = 1;
                }
                // check xem tin nhắn đã gửi hay chưa, nếu chưa thì nhập tin nhắn phản hồi cho client
                if (check == 0 && send == 0) {
                    System.out.println("sai o dong nay");
                    buff = "";
                    int n = 0;
                    while ((n = System.in.read()) != -1) {
                        buff += (char) n;
                        if (n == '\n') break;
                    }
                    out.writeUTF(buff);
                }
                if (send == 1) {
                    send = 0;
                }
                if (buff.equals("exit")) {
                    System.out.println("Server Exit...");
                    break;
                }
            }
            // đóng kết nối
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (clientSocket != null) {
                    clientSocket.close();
                }
                if (serverSocket != null) {
                    serverSocket.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
