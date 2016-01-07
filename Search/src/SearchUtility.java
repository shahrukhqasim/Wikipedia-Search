import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

/**
 * Created by shahrukhqasim2 on 1/5/2016.
 */
public class SearchUtility  extends JFrame{
    SearchUtility() {
        try {
            UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
            setSize(new Dimension(800,450));
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        ImageIcon icon = new ImageIcon("icon.png");
        setIconImage(icon.getImage());

        setTitle("Wikipedia Search");

        Container pane=getContentPane();

        if (!(pane.getLayout() instanceof BorderLayout)) {
            pane.add(new JLabel("Container doesn't use BorderLayout!"));
            return;
        }

        pane.add(new SearchRow(), BorderLayout.PAGE_START);
        pane.add(new ResultsPane(), BorderLayout.CENTER);
        pane.add(new NavigationPane(), BorderLayout.PAGE_END);
    }

    public static void main(String[]args) {
        try {
            UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        SearchUtility gui=new SearchUtility();
        gui.setVisible(true);
        gui.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
