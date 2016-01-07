import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;

/**
 * Created by shahrukhqasim2 on 1/6/2016.
 */
public class URLServer {
    public static final String docListPath="D:\\Workspace\\Wikipedia\\Parser\\docIds.txt";
    HashMap<Integer,String> docList=new HashMap<>();

    private void loadDocList() throws Exception{
        try {
            BufferedReader lexiconInputStream = new BufferedReader(new FileReader(docListPath));
            while(lexiconInputStream.ready()) {
                String line = lexiconInputStream.readLine();
                String[]data = line.split(",");
                if(data.length>=2) {
                    docList.put(Integer.parseInt(data[0]), data[1]);
                }
            }
        }
        catch(Exception e){
            System.out.println("Error in doc list");
            e.printStackTrace();
            throw new Exception("Lexicon");
        }
    }

    public void execute() throws Exception {
        System.out.println("Loading docList");
        loadDocList();
        System.out.println("Loaded");
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(14578);
        } catch (IOException e) {
            System.err.println("Could not listen on port: 4444.");
            System.exit(1);
        }

        Socket clientSocket = null;
        while(true) {
            try {
                System.out.println("Waiting for someone to connect");
                clientSocket = serverSocket.accept();
                System.out.println("Connected");

                PrintStream printStream=new PrintStream(clientSocket.getOutputStream());

                DataInputStream in=new DataInputStream(clientSocket.getInputStream());
                int n=in.readInt();

//                for(int i=0;i<n;i++) {
//                    int x=in.readInt();
                    if(docList.containsKey(n))
                        printStream.println(docList.get(n));
                    else
                        printStream.print("");
//                }
                in.close();
                printStream.close();

                clientSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
                System.err.println("Accept failed.");
            }
        }

//        serverSocket.close();
    }

    public static void main(String[]args) {
       try {
           new URLServer().execute();
       }
       catch (Exception e) {
           e.printStackTrace();
       }
    }
}
