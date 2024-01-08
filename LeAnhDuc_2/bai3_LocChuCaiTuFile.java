//Họ và tên : Lê Anh Đức
//MSV : 21020761

import java.io.*;
import java.util.Scanner;

public class bai3_LocChuCaiTuFile {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Nhập tên file nguồn
        System.out.print("Nhập tên file nguồn: ");
        String inputFile = scanner.nextLine();

        // Nhập tên file đích
        System.out.print("Nhập tên file đích: ");
        String outputFile = scanner.nextLine();

        try {
            // Tạo đối tượng FileReader để đọc file nguồn
            FileReader fileReader = new FileReader(inputFile);
            BufferedReader bufferedReader = new BufferedReader(fileReader);

            // Tạo đối tượng FileWriter để ghi file đích
            FileWriter fileWriter = new FileWriter(outputFile);

            int c;
            //lọc chữ cái từ file nguồn để copy sang file đích
            while ((c = bufferedReader.read()) != -1) {
                char character = (char) c;
                if (Character.isLetter(character)) {
                    fileWriter.write(character);
                }
            }

            // Đóng các luồng
            bufferedReader.close();
            fileWriter.close();

            System.out.println("Dữ liệu đã được lọc và lưu vào file đích thành công.");
        } catch (IOException e) {
            System.err.println("Đã xảy ra lỗi: " + e.getMessage());
        }
    }
}
