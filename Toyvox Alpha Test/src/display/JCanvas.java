package display;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import javax.swing.JPanel;

public class JCanvas extends JPanel {
    
    private BufferedImage backBuffer;
    
    //Initializes the object
    public void init(){
        
        backBuffer = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_ARGB);
    }
    
    public BufferedImage getBackBuffer(){
        return backBuffer;
    }
    
    public void setImage(BufferedImage image){
        
        backBuffer = image;
    }
    
    //Draws the back buffer into the canvas.
    public void draw(){
        
        getGraphics().drawImage(backBuffer, 0, 0, this);
    }

    /* Special Functions
        -- Those will make the drawing easier.
    */
    
    public void setPixel(int x, int y, byte[] color){
        
        getGraphics().setColor(new Color(color[0], color[1], color[2], color[3]));
        getGraphics().drawLine(x, y, x, y);
    }
    
    public Graphics2D getBufferGraphics(){
        
        return (Graphics2D) backBuffer.getGraphics();
    }
}
