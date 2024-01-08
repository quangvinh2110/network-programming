//Họ và tên : Lê Anh Đức
//MSV : 21020761

import java.io.*;
import java.security.MessageDigest;
import java.util.Scanner;

public class bai4_CopyAndCheckMD5 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Nhập tên file nguồn
        System.out.print("Nhập tên file nguồn: ");
        String sourceFileName = scanner.nextLine();

        // Nhập tên file đích
        System.out.print("Nhập tên file đích: ");
        String destinationFileName = scanner.nextLine();

        try {
            // Đọc dữ liệu từ file nguồn và ghi vào file đích
            FileInputStream sourceFileStream = new FileInputStream(sourceFileName);
            FileOutputStream destinationFileStream = new FileOutputStream(destinationFileName);

            byte[] buffer = new byte[1024];
            int bytesRead;
            while ((bytesRead = sourceFileStream.read(buffer)) != -1) {
                destinationFileStream.write(buffer, 0, bytesRead);
            }

            sourceFileStream.close();
            destinationFileStream.close();

            System.out.println("Sao chép file thành công.");

            // Tính toán MD5 của cả hai file
            String md5Source = calculateMD5(sourceFileName);
            String md5Destination = calculateMD5(destinationFileName);

            // So sánh MD5 của hai file
            if (md5Source.equals(md5Destination)) {
                System.out.println("Dữ liệu của hai file giống nhau.");
            } else {
                System.out.println("Dữ liệu của hai file khác nhau.");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String calculateMD5(String fileName) {
        try {
            //Tạo đối tượng MessageDigest với thuật toán MD5
            MessageDigest md = MessageDigest.getInstance("MD5");

            //Đọc dữ liệu từ file vào bộ đệm và cập nhật MessageDigest
            FileInputStream fis = new FileInputStream(fileName);
            byte[] dataBytes = new byte[1024]; //Bộ đệm cho dữ liệu từ file
            int bytesRead;// số bytes cho mỗi lần đọc

            while ((bytesRead = fis.read(dataBytes)) != -1) {
                md.update(dataBytes, 0, bytesRead); //Cập nhật MessageDigest với dữ liệu đọc được
            }
            //Tính toán mã băm MD5 của dữ liệu
            byte[] mdBytes = md.digest();
            //Chuyển đổi mã băm MD5 thành chuỗi hex
            StringBuilder sb = new StringBuilder();
            for (byte mdByte : mdBytes) {
                sb.append(Integer.toString((mdByte & 0xff) + 0x100, 16).substring(1));
            }
            //Trả về chuỗi hex đại diện cho mã băm MD5
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
