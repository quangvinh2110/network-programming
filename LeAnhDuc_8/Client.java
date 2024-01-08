//21020761 Le Anh Duc
import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) {
        Socket socket = null;
        DataOutputStream dos = null;
        BufferedReader br = null;

        try {
            // Nhập địa chỉ IP của Server từ bàn phím
            System.out.print("Enter server IP address: ");
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            String serverIP = reader.readLine();

            // Nhập tên file từ bàn phím
            System.out.print("Enter the file name: ");
            String fileName = reader.readLine();

            // Kết nối với server
            socket = new Socket(serverIP, 12345);
            dos = new DataOutputStream(socket.getOutputStream());
            br = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            // Gửi tên file cho server
            dos.writeBytes("FILENAME " + fileName + "\n");

            // Nhận thông tin về file từ server
            String fileInfo = br.readLine();
            String[] fileInfoArray = fileInfo.split(" ");
            long fileSize = Long.parseLong(fileInfoArray[2]);

            // Xuất thông tin về file
            System.out.println("File Name: " + fileName);
            System.out.println("File Size: " + fileSize + " bytes");

            // Gửi xác nhận cho server
            dos.writeBytes("READY\n");

            // Nhận nội dung file và lưu vào thư mục hiện tại
            try (FileOutputStream fos = new FileOutputStream(fileName)) {
                byte[] buffer = new byte[1024];
                int bytesRead;

                while ((bytesRead = socket.getInputStream().read(buffer)) != -1) {
                    fos.write(buffer, 0, bytesRead);
                }

                System.out.println("File received successfully.");
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (dos != null) dos.close();
                if (br != null) br.close();
                if (socket != null) socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
