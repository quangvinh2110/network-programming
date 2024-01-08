//Họ và tên : Lê Anh Đức
//MSV : 21020761


import java.util.Scanner;

public class bai1_DemSoKiTu {
    public static void main(String[] args) {
        // nhập chuỗi
        Scanner scanner = new Scanner(System.in);
        System.out.print("Nhập chuỗi: ");
        String input = scanner.nextLine();

        int chuCai = 0;
        int so = 0;
        int kyHieu = 0;

        // đếm số các chữ cái, số, ký hiệu
        for (int i = 0; i < input.length(); i++) {
            char kyTu = input.charAt(i);
            if (Character.isLetter(kyTu)) {
                chuCai++;
            } else if (Character.isDigit(kyTu)) {
                so++;
            } else {
                kyHieu++;
            }
        }

        System.out.println("Số ký tự là chữ cái: " + chuCai);
        System.out.println("Số ký tự là số: " + so);
        System.out.println("Số ký tự là ký hiệu: " + kyHieu);
    }
}
