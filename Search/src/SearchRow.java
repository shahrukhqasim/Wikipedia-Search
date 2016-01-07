import javax.swing.*;
import java.awt.*;

/**
 * Created by shahrukhqasim2 on 1/5/2016.
 */
public class SearchRow extends JPanel {
    SearchRow() {
        setLayout(new FlowLayout());
        JTextField textField=new JTextField();
        SearchDataManager.get().setSearchEditBox(textField);
        textField.setColumns(50);
        JButton enterButton=new JButton();
        SearchDataManager.get().setSearchButton(enterButton);
        enterButton.setText("Search");
        add(textField);
        add(enterButton);
    }
}
