package engineTester;

import display.JCanvas;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;
import space3d.Layout3D;

import static java.lang.Math.*;
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
        setSize(640, 360);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setResizable(false);

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

    Sphere obj = new Sphere(1, new Vector(0, 0, 0));
    Camera cam = new Camera(new Vector(10, 10, 0), -45, -45, 80, 45);
    
    JCanvas canvas = new JCanvas();
    Layout3D layout = new Layout3D();
    private final byte fps = 60;

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
