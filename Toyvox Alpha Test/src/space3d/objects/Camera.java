package space3d.objects;

import engineTester.Sphere;
import java.awt.Color;
import java.awt.Graphics2D;
import static space3d.VMathUtils.*;
import java.awt.Image;
import java.awt.image.BufferedImage;
import space3d.Layout3D;
import space3d.Object3D;
import space3d.VMathUtils;
import space3d.Vector;

public class Camera extends Object3D {

    public double gethROT() {
        return hROT;
    }

    public void sethROT(double hROT) {
        this.hROT = angleBind(hROT);
    }

    public double getvROT() {
        return vROT;
    }

    public void setvROT(double vROT) {
        this.vROT = angleBind(vROT);
    }

    public double gethFOV() {
        return hFOV;
    }

    public void sethFOV(double hFOV) {
        this.hFOV = angleBind(hFOV);
    }

    public double getvFOV() {
        return vFOV;
    }

    public void setvFOV(double vFOV) {
        this.vFOV = angleBind(vFOV);
    }

    /**
     * @param hROT the Horizontal Rotation of the camera
     * @param vROT the Vertical Rotation of the camera
     * @param hFOV the Horizontal F.O.V. of the camera
     * @param vFOV the Vertical F.O.V. of the camera
     */
    private double hROT, vROT, hFOV, vFOV;

    public BufferedImage render(Layout3D layout, int width, int height) {

        BufferedImage output = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = (Graphics2D) output.getGraphics();

        double rayAngleH, rayAngleV, x, y, a, b, px, py;

        //For every pixel in the screen, shoot a ray and check if it intersects with something.
        //In this case, we'll check if it intersects with the 2nd object of the Layout,
        //that's supposed to be a Sphere.
        for (int ky = 0; ky < height; ky++) {
            for (int kx = 0; kx < width; kx++) {

                rayAngleH = angleLerp(hROT - hFOV / 2, hROT + hFOV / 2, getP(0, kx, width));
                rayAngleV = angleLerp(vROT - vFOV / 2, vROT + vFOV / 2, getP(0, ky, height));

                Sphere sphere = (Sphere) layout.getObject(0);
                
                Vector sphereCenter = sphere.getPosition();
                Vector ray = add(normalize(getRectVector(10, rayAngleH, rayAngleV)), getPosition());
                
                double dotproduct = dotProduct(sphereCenter, ray);
                //System.out.println(ray.toString() + " . " + sphere.getPosition().toString() + " = " + dotproduct);
                
                Vector closestPoint = multiply(ray, dotproduct);
                
                double distance = distance(closestPoint, sphereCenter);
                //System.out.println(ray.toString() + " " + closestPoint.toString());
                
                //May cause problems later
                if(distance <= (sphere.getRadius()) && dotproduct >= 0) {
                    
                    g.setColor(sphere.getColor());
                } else {
                    
                    g.setColor(new Color(100, 100, 100));
                }
                
                //Sets the pixel to the chosen color
                g.drawLine(kx, ky, kx, ky);
            }
        }

        return output;
    }

    public Camera(Vector position, double hROT, double vROT, double hFOV, double vFOV) {

        this.hROT = angleBind(hROT);
        this.vROT = angleBind(vROT);
        this.hFOV = angleBind(hFOV);
        this.vFOV = angleBind(vFOV);

        setPosition(position);
    }
}
