//Họ và tên : Lê Anh Đức
//MSV : 21020761

import java.io.*;
import java.util.Scanner;

public class bai2_GhiChuoiVaoFile {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Nhập tên file từ người dùng
        System.out.print("Nhập tên file: ");
        String fileName = scanner.nextLine();

        try {
            // Tạo một đối tượng FileWriter để ghi vào file
            FileWriter fileWriter = new FileWriter(fileName);
            BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);

            System.out.println("Nhập các chuỗi ký tự (Nhập 'exit' để kết thúc):");
            String input;
            while (true) {
                input = scanner.nextLine();
                if (input.equalsIgnoreCase("exit")) {
                    break;
                }
                bufferedWriter.write(input);
                bufferedWriter.newLine();
            }

            bufferedWriter.close();
            System.out.println("Đã ghi vào file thành công.");

            // Lấy kích thước file
            File file = new File(fileName);
            long fileSize = file.length();
            System.out.println("Kích thước file là " + fileSize + " byte.");

        } catch (IOException e) {
            System.out.println("Đã xảy ra lỗi khi ghi vào file: " + e.getMessage());
        }
    }
}
