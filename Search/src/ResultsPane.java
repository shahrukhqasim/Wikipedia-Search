import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;

/**
 * Created by shahrukhqasim2 on 1/5/2016.
 */
public class ResultsPane extends JPanel {
    JLinkButton []results=new JLinkButton[10];
    JLabel []resultsTitles=new JLabel[10];
    JLabel status;
    public ResultsPane() {
        setLayout(new BoxLayout(this,BoxLayout.Y_AXIS));
        setBorder(new EmptyBorder(30,30,30,30));
        status=new JLabel("No results found");
        status.setHorizontalAlignment(JLabel.CENTER);
        SearchDataManager.get().setResultsStatusLabel(status);
        status.setForeground(Color.ORANGE);
        add(status);
        for(int i=0;i<10;i++) {
            JLabel title=new JLabel("Facebook");
            add(title);
            JLinkButton button=new JLinkButton("http://www.facebook.com/");
            add(button);
            results[i]=button;
            resultsTitles[i]=title;
        }
        SearchDataManager.get().setResultsPane(this);
        setBackground(Color.WHITE);
    }

    public void setResultAt(int index, String title, String url) {
        results[index].setText(url);
        resultsTitles[index].setText(title);
    }
}
