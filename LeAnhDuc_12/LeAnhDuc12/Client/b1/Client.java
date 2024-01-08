// 21020761 Le Anh Duc
import java.io.*;
import java.net.Socket;

public class Client {
    public static void main(String[] args) {
        try {
            BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));

            System.out.print("Enter Server IP: ");
            String serverIP = consoleReader.readLine();

            System.out.print("Enter User ID: ");
            String userID = consoleReader.readLine();

            Socket socket = new Socket(serverIP, 12345);

            // Send User ID to the server
            ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
            outputStream.writeObject(userID);

            // Start reading messages from the server
            new Thread(new ServerListener(socket)).start();

            // Allow user to send messages
            while (true) {
                System.out.print("Enter message: ");
                String message = consoleReader.readLine();
                outputStream.writeObject(message);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

