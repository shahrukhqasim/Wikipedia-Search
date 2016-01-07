import javax.swing.*;

/**
 * Created by shahrukhqasim2 on 1/5/2016.
 */
public class NavigationPane extends JPanel {
    NavigationPane() {
        JButton previousButton=new JButton("Previous");
        SearchDataManager.get().setPreviousButton(previousButton);
        add(previousButton);
        JLabel pageNumber=new JLabel("Page 1");
        SearchDataManager.get().setPageNumberLabel(pageNumber);
        add(pageNumber);
        JButton nextButton=new JButton("Next");
        SearchDataManager.get().setNextButton(nextButton);
        add(nextButton);

        SearchDataManager.get().init();
    }
}
