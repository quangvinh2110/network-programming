// 21020761 Le Anh Duc
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ClientHandler implements Runnable {
    private Socket socket;
    private ObjectOutputStream outputStream;

    public ClientHandler(Socket socket) {
        this.socket = socket;
        try {
            outputStream = new ObjectOutputStream(socket.getOutputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            ObjectInputStream inputStream = new ObjectInputStream(socket.getInputStream());

            // Receive User ID from the client
            String userID = (String) inputStream.readObject();
            System.out.println("User " + userID + " has joined the chat.");

            // Broadcast the new user to all clients
            Server.broadcastMessage("User " + userID + " has joined the chat.", this);

            // Send the list of online users to the new client
            sendMessage("Online users: " + Server.clients.toString());

            // Start listening for messages
            while (true) {
                String message = (String) inputStream.readObject();
                Server.broadcastMessage(userID + ": " + message, this);
            }
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        } finally {
            Server.removeClient(this);
            try {
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    void sendMessage(String message) {
        try {
            outputStream.writeObject(message);
            outputStream.flush();  // Ensure data is sent immediately
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

