//21020761  Le Anh Duc
import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        Socket clientSocket = null;
        DataOutputStream dos = null;
        BufferedReader br = null;

        try {
            // Mở cổng chờ kết nối TCP từ client (port 12345)
            serverSocket = new ServerSocket(12345);

            // Chờ client kết nối
            System.out.println("Waiting for client connection...");
            clientSocket = serverSocket.accept();
            System.out.println("Client connected.");

            // Tạo luồng vào/ra cho client
            dos = new DataOutputStream(clientSocket.getOutputStream());
            br = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

            // Nhận tên file từ client
            String clientRequest = br.readLine();
            String[] requestArray = clientRequest.split(" ");
            if (requestArray[0].equals("FILENAME")) {
                String fileName = requestArray[1];

                // Kiểm tra sự tồn tại của file
                File file = new File(fileName);
                if (!file.exists()) {
                    System.out.println("File not found: " + fileName);
                    dos.writeBytes("ERROR\n");
                    return;
                }

                long fileSize = file.length();

                // Gửi tên và kích thước file cho client
                dos.writeBytes("FILEINFO " + fileName + " " + fileSize + "\n");

                // Nhận xác nhận từ client
                String clientResponse = br.readLine();
                if (clientResponse.equals("READY")) {
                    System.out.println("Client is ready to receive the file.");

                    // Gửi nội dung file cho client
                    try (FileInputStream fis = new FileInputStream(file)) {
                        byte[] buffer = new byte[1024];
                        int bytesRead;

                        while ((bytesRead = fis.read(buffer)) != -1) {
                            dos.write(buffer, 0, bytesRead);
                        }

                        System.out.println("File sent successfully.");
                    }
                }
            } else {
                System.out.println("Invalid request from client.");
                dos.writeBytes("ERROR\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (dos != null) dos.close();
                if (br != null) br.close();
                if (clientSocket != null) clientSocket.close();
                if (serverSocket != null) serverSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
