package engineTester;

import display.JCanvas;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;
import space3d.Layout3D;

import static java.lang.Math.*;
import javax.imageio.ImageIO;
import space3d.VMathUtils;
import static space3d.VMathUtils.*;
import space3d.Vector;
import space3d.objects.Camera;
import space3d.objects.TransformableObject;

/**
 *
 * @author Pedro Braga
 */
public class AppStart extends JFrame {

    /**
     * It's the constructor!
     */
    public AppStart() {
        setTitle("Toyvox Engine Test! Version 0.0.1");
        setSize(320, 180);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setResizable(false);

        try {
            setIconImage(ImageIO.read(AppStart.class.getResource("enginelogo.png")));
        } catch (Exception e){
            System.err.println("Couldn't find icon image.");
        }
        
        add(canvas, BorderLayout.CENTER);
        canvas.setBackground(new Color(100, 170, 200));
        setVisible(true);

        canvas.init();

        //Add objects to scene
        
        layout.addObject(obj);
        layout.addObject(cam);
        
        //Start
        start();
    }

    Sphere obj = new Sphere(4, new Vector(-100, 0, 0), new Color(100, 200, 255));
    Sphere obj2 = new Sphere(4, new Vector(-100, 10, 0), new Color(255, 200, 255));
    Camera cam = new Camera(new Vector(0, 0, 0), -90, 45, 90, 45);
    
    JCanvas canvas = new JCanvas();
    Layout3D layout = new Layout3D();
    private final byte fps = 120;

    public void start() {

        while (true) {

            long time = System.currentTimeMillis();

            //Draws stuff at the screen
            Graphics2D g = canvas.getBufferGraphics();

            //DRAWS BACKGROUND
            g.setColor(canvas.getBackground());
            
            g.drawImage(cam.render(layout, canvas.getWidth(), canvas.getHeight()), 0, 0, canvas);

            //Draws the backBuffer at the screen.
            canvas.draw();
            //cam.sethROT(cam.gethROT() + (double)fps/20);
            obj.move(new Vector(1, 0, 0));
            
            //Syncronize with the fps meter.
            time = System.currentTimeMillis() - time;

            if (time < 1000 / fps) {
                try {
                    Thread.sleep(1000 / fps - time);
                } catch (InterruptedException ex) {
                    Logger.getLogger(AppStart.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        new AppStart();
    }
}
