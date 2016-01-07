import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Vector;

/**
 * Created by shahrukhqasim2 on 1/5/2016.
 */
public class SearchDataManager {
    public static final String lexiconPath="D:\\Workspace\\Wikipedia\\Parser2\\Lexicon.txt";
    public static final String docListPath="D:\\Workspace\\Wikipedia\\Parser2\\docIds.txt";
    public static final String[] reversePaths={"D:\\Workspace\\Wikipedia\\Parser2\\S\\titles\\","D:\\Workspace\\Wikipedia\\Parser2\\S\\sub2\\","D:\\Workspace\\Wikipedia\\Parser2\\S\\sub3\\","D:\\Workspace\\Wikipedia\\Parser2\\S\\sub4\\","D:\\Workspace\\Wikipedia\\Parser2\\S\\sub5\\","D:\\Workspace\\Wikipedia\\Parser2\\S\\normal\\"};
    HashMap<String, Integer> lexicon=new HashMap<>();
    HashMap<Integer,String> docList=new HashMap<>();

    Vector<Integer>resultsStored=new Vector<>();
    int numResults=0;
    double time;

    String queryText;

    private HashSet<Integer>loadDocs(int wordId, int level) {
        HashSet<Integer>docList=new HashSet<>();
        try {
            DataInputStream inputStream = new DataInputStream(new FileInputStream(reversePaths[level]+wordId+".txt"));
            while (inputStream.available()>0) {
                docList.add(Integer.reverseBytes(inputStream.readInt()));
            }
        }
        catch (EOFException e) {

        }
        catch(Exception e) {
            return new HashSet<>();
        }
        return docList;
    }

    private void loadDocList() throws Exception{
        try {
            BufferedReader lexiconInputStream = new BufferedReader(new FileReader(docListPath));
            while(lexiconInputStream.ready()) {
                String line = lexiconInputStream.readLine();
                String[]data = line.split(",");
                docList.put(Integer.parseInt(data[0]),data[1]);
            }
        }
        catch(Exception e){
            System.out.println("Error in doc list");
            e.printStackTrace();
            throw new Exception("Lexicon");
        }
    }

    private void loadLexicon() throws Exception{
        try {
            BufferedReader lexiconInputStream = new BufferedReader(new FileReader(lexiconPath));
            while(lexiconInputStream.ready()) {
                String line = lexiconInputStream.readLine();
                String[]data = line.split(",");
                lexicon.put(data[0],Integer.parseInt(data[1]));
            }
        }
        catch(Exception e){
            System.out.println("Error in lexicon");
            e.printStackTrace();
            throw new Exception("Lexicon");
        }
    }

    public void setQueryText(String queryText) {
        this.queryText=queryText;
    }

    public void handleQuery() {
        resultsStored.clear();
        String[] searchWords=queryText.toLowerCase().split(" ");
        if(searchWords.length==0) {
            System.err.println("Error");
        }
        Vector<Integer>wordIds=new Vector<>();
        for(int i=0;i<searchWords.length;i++) {
            wordIds.add(lexicon.get(searchWords[i]));
        }
        System.out.println(wordIds);
        for(int k=0;k<6;k++) {
            HashSet[] resultsDocIds = new HashSet[searchWords.length];
            for (int i = 0; i < searchWords.length; i++) {
                int wordId = wordIds.get(i);
                resultsDocIds[i] = loadDocs(wordId, k);
            }
            HashSet<Integer> result = resultsDocIds[0];
            for (int i = 1; i < resultsDocIds.length; i++) {
                result.retainAll(resultsDocIds[i]);
            }
            resultsStored.addAll(result);
        }

        numResults=resultsStored.size();
    }

    public void search() {
        try {
            long startTime=System.nanoTime();
            handleQuery();
            long endTime=System.nanoTime();
            time=(double)(endTime-startTime)/1000000000;
        }
        catch (Exception e){
            e.printStackTrace();
            System.err.println(e.getMessage());
        }
    }
    SearchDataManager() {
        try {
            loadLexicon();
            System.out.println("Lexicon loaded successfully");

            loadDocList();
            System.out.println("DocList loaded successfully");
        }
        catch (Exception e) {
            e.printStackTrace();
            System.err.println(e.getMessage());
        }
    }

    public void init() {
        setPageNumber(1);
    }


    JTextField inputField;
    JButton searchButton;
    ResultsPane resultsPane;
    JButton nextButton;
    JButton previousButton;
    JLabel pageNumberLabel;
    JLabel resultsStatusLabel;
    int pageNumber=1;

    public void setResultsStatusLabel(JLabel resultsStatusLabel) {
        this.resultsStatusLabel = resultsStatusLabel;
    }

    public void setPageNumberLabel(JLabel pageNumberLabel) {
        this.pageNumberLabel =pageNumberLabel;
    }

    public void setSearchEditBox(JTextField inputField) {
        this.inputField=inputField;
    }

    public void setResultsPane(ResultsPane resultsPane) {
        this.resultsPane=resultsPane;
    }

    String getDocText(int docId) {
        return docList.get(docId);
    }

    private void setPageNumber(int pageNumber) {
        int startIndex=(pageNumber-1)*10;
        int i=0;
        try {
            for (i = 0; i < 10; i++) {
                String docText = getDocText(resultsStored.get(i + startIndex));
                // e.g. https://simple.wikipedia.org/wiki/Dutch_language
                String url = "https://simple.wikipedia.org/wiki/" + docText.replaceAll(" ", "_");
                resultsPane.setResultAt(i, docText,url);
            }
            nextButton.setEnabled(true);
        }
        catch (ArrayIndexOutOfBoundsException e) {
            for(;i<10;i++) {
                resultsPane.setResultAt(i,"","");
            }
            nextButton.setEnabled(false);
        }

        if(pageNumber==1)
            previousButton.setEnabled(false);
        else {
            previousButton.setEnabled(true);
        }


        this.pageNumber=pageNumber;
        pageNumberLabel.setText("Page "+pageNumber);
        if(numResults!=0) {
            resultsStatusLabel.setText(numResults + " results in " +time+" seconds");
        }
        else {
            resultsStatusLabel.setText("No results found");
        }
    }

    public void setSearchButton(JButton searchButton) {
        this.searchButton=searchButton;
        searchButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                queryText=inputField.getText();
                search();
                setPageNumber(1);
            }
        });
    }
    public void setNextButton(JButton nextButton) {
        this.nextButton=nextButton;
        nextButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                setPageNumber(pageNumber+1);
            }
        });
    }

    public void setPreviousButton(JButton previousButton) {
        this.previousButton=previousButton;
        previousButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                setPageNumber(pageNumber-1);
            }
        });
    }

    private static SearchDataManager reference;

    public static SearchDataManager get() {
        if(reference==null)
            reference=new SearchDataManager();
        return reference;
    }
}
