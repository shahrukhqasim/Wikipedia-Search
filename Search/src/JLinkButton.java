import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.font.TextAttribute;
import java.net.URL;
import java.util.Map;

import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.SwingConstants;
import javax.swing.WindowConstants;

/**
 * JLinkButton draws a button like JLabel. Default foreground color is blue and the text is underlined on mouse hover
 * (behaviour depends on {@link #underlineMode}). Shows the hand cursor.
 *
 * @author xmedeko
 */
public class JLinkButton extends JButton {

    /**
     * Type of underline mode.
     */
    public enum UnderlineMode {
        NONE, ALWAYS, HOVER
    };

    private UnderlineMode underlineMode = UnderlineMode.HOVER;

    public JLinkButton() {
        super();
    }

    public JLinkButton(Action a) {
        super(a);
    }

    public JLinkButton(Icon icon) {
        super(icon);
    }

    public JLinkButton(String text, Icon icon) {
        super(text, icon);
    }

    public JLinkButton(String text) {
        super(text);
    }

    @Override
    protected void init(String text, Icon icon) {
        super.init(text, icon);

        setFocusPainted(false);
        setMargin(null);
        setContentAreaFilled(false);
        setBorderPainted(false);
        setBorder(BorderFactory.createEmptyBorder());
        setOpaque(false);
        setHorizontalAlignment(SwingConstants.LEFT);
        setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));

        setForeground(Color.BLUE);

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                if (getUnderlineMode() == UnderlineMode.HOVER) {
                    setUnderlineFont(true);
                }
            }

            @Override
            public void mouseExited(MouseEvent e) {
                if (getUnderlineMode() == UnderlineMode.HOVER) {
                    setUnderlineFont(false);
                }
            }

            @Override
            public void mouseClicked(MouseEvent mouseEvent) {
                System.out.println("Loading");
                try {
                    Desktop.getDesktop().browse(new URL(getText()).toURI());
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
                super.mouseClicked(mouseEvent);
            }
        });
    }


    /**
     * set or reset the underline font.
     */
    protected void setUnderlineFont(boolean underline) {
        @SuppressWarnings("unchecked")
        Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) getFont().getAttributes();
        if (underline) {
            map.put(TextAttribute.UNDERLINE, TextAttribute.UNDERLINE_ON);
        } else {
            map.remove(TextAttribute.UNDERLINE);
        }
        Font newFont = new Font(map);
        setFont(newFont);
    }

    /**
     * @return underline mode
     */
    public UnderlineMode getUnderlineMode() {
        return underlineMode;
    }

    /**
     * Set underline mode.
     *
     * @return this, just for fluent API.
     */
    public JLinkButton setUnderlineMode(UnderlineMode underlineMode) {
        this.underlineMode = underlineMode;
        switch (underlineMode) {
            case ALWAYS:
                setUnderlineFont(true);
                break;
            case NONE:
                setUnderlineFont(false);
                break;
            case HOVER: // nothing
                break;
        }

        return this;
    }

    // test
    public static void main(String[] args) {
        JFrame frame = new JFrame();
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setLayout(new FlowLayout());
        frame.add(new JLinkButton("Hover").setUnderlineMode(UnderlineMode.HOVER));
        frame.add(new JLinkButton("None").setUnderlineMode(UnderlineMode.NONE));
        frame.add(new JLinkButton("Always").setUnderlineMode(UnderlineMode.ALWAYS));
        JLinkButton disabled = new JLinkButton("Disabled");
        disabled.setEnabled(false);
        frame.add(disabled);

        frame.pack();
        frame.setVisible(true);
    }

}