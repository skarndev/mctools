package artofillusion.translators;

import artofillusion.*;
import artofillusion.math.*;
import artofillusion.object.*;
import artofillusion.texture.*;
import artofillusion.ui.*;
import buoy.event.*;
import buoy.widget.*;

import java.io.*;
import java.nio.ByteBuffer;
import java.text.*;
import java.util.*;

import de.taliis.editor.openedFile;

import starlight.taliis.core.chunks.ChunkNotFoundException;
import starlight.taliis.core.chunks.wmo.group.MOBA_Entry;
import starlight.taliis.core.chunks.wmo.group.MOBN_Entry;
import starlight.taliis.core.files.m2;
import starlight.taliis.core.files.wowfile;
import starlight.taliis.helpers.fileLoader;


/** WMOExporter contains the actual routines for exporting OBJ files. */
public class M2Exporter
{
	static File f;
	/** Display a dialog which allows the user to export a scene to an OBJ file. */
	  
	  public static void exportFile(BFrame parent, Scene theScene)
	  {
		// Display a dialog box with options on how to export the scene.
		    
		    //ValueField errorField = new ValueField(0.05, ValueField.POSITIVE);
		    //final ValueField widthField = new ValueField(200.0, ValueField.INTEGER+ValueField.POSITIVE);
		    //final ValueField heightField = new ValueField(200.0, ValueField.INTEGER+ValueField.POSITIVE);
		    //final ValueSlider qualitySlider = new ValueSlider(0.0, 1.0, 100, 0.5);
		    //final BCheckBox smoothBox = new BCheckBox(Translate.text("subdivideSmoothMeshes"), true);
		    final BCheckBox injectChoice = new BCheckBox(Translate.text("Inject"), true);
		   // final BCheckBox UVChoice = new BCheckBox(Translate.text("Export UV"), true);
		    
		    
		    /*BComboBox exportChoice = new BComboBox(new String [] {
		      Translate.text("exportWholeScene"),
		      Translate.text("selectedObjectsOnly")
		    });*/
		    
		    
		     
		      ComponentsDialog dlg;
		      if (theScene.getSelection().length > 0)
		        dlg = new ComponentsDialog(parent, Translate.text("InjectToM2"), 
		  	  new Widget [] {   injectChoice},
		  	  new String [] {null, null, null, null, null});
		      else
		        dlg = new ComponentsDialog(parent, Translate.text("exportToM2"), 
		  	  new Widget [] {  injectChoice},
		  	  new String [] {null, null, null, null});
		      if (!dlg.clickedOk())
		        return;
		      
		   // Ask the user to select the output file.

		      BFileChooser fc = new BFileChooser(BFileChooser.SAVE_FILE, Translate.text("exportToWMO"));
		      fc.setSelectedFile(new File("Untitled.m2"));
		      if (ArtOfIllusion.getCurrentDirectory() != null)
		        fc.setDirectory(new File(ArtOfIllusion.getCurrentDirectory()));
		      if (!fc.showDialog(parent))
		        return;
		      File dir = fc.getDirectory();
		      f = fc.getSelectedFile();
		      String name = f.getName();
		      String baseName = (name.endsWith(".m2") ? name.substring(0, name.length()-3) : name);
		      ArtOfIllusion.setCurrentDirectory(dir.getAbsolutePath());

		      m2 obj = null;
		      try {
				obj= new m2( fileLoader.openBuffer(f.getAbsolutePath()));
		      } catch (InvalidClassException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
		      }
		      writeScene(theScene, obj, false, 0.05, false, injectChoice.getState()); 
	  }
	  public static void writeScene(Scene theScene, m2 out, boolean wholeScene, double tol, boolean smooth, boolean inject)
	  {
		// Write the objects in the scene.

		    int numVert = 0, numNorm = 0, numTexVert = 0;
		    Hashtable<String, String> groupNames = new Hashtable<String, String>();
		    NumberFormat nf = NumberFormat.getNumberInstance(Locale.US);
		    nf.setMaximumFractionDigits(5);
		    nf.setGroupingUsed(false);  
		    for (int i = 0; i < theScene.getNumObjects(); i++)
		      {
		        // Get a rendering mesh for the object.

		        ObjectInfo info = theScene.getObject(i);
		        if (!wholeScene && !info.selected)
		          continue;
		        if (info.getObject().getTexture() == null)
		          continue;
		        FacetedMesh mesh;

		        if (!smooth && info.getObject() instanceof FacetedMesh)
		            mesh = (FacetedMesh) info.getObject();
		          else
		            mesh = info.getObject().convertToTriangleMesh(tol);
		          if (mesh == null)
		            continue;

		       // Find the normals.
		          
		          Vec3 norm[];
		          int normIndex[][] = new int[mesh.getFaceCount()][];
		          if (mesh instanceof TriangleMesh)
		          {
		            RenderingMesh rm = ((TriangleMesh) mesh).getRenderingMesh(Double.MAX_VALUE, false, info);
		            norm = rm.norm;
		            for (int j = 0; j < normIndex.length; j++)
		              normIndex[j] = new int[] {rm.triangle[j].n1, rm.triangle[j].n2, rm.triangle[j].n3};
		          }
		          else
		          {
		            norm = mesh.getNormals();
		            for (int j = 0; j < normIndex.length; j++)
		            {
		              normIndex[j] = new int[mesh.getFaceVertexCount(j)];
		              for (int k = 0; k < normIndex[j].length; k++)
		                normIndex[j][k] = mesh.getFaceVertexIndex(j, k);
		            }
		          }
		       // Write out the object.
		          
		          
		          Mat4 trans = info.getCoords().fromLocal();
		          MeshVertex vert[] = mesh.getVertices();
		          
		          System.out.println("nVertixes:\t"+out.vertex.length);
		          for (int j = 0; j < vert.length; j++)
		            {
		              Vec3 v = trans.times(vert[j].r);
		              float[] tf= new float[3];
		              tf[0]=(float) v.x;
		              tf[1]=(float) v.y;
		              tf[2]=(float) v.z;
		              out.vertex[j].setVertexPos(tf);
		              
		            }
		          /*short faceCount[][]=new short[mesh.getFaceCount()][3];
		          for(int f=0;f<mesh.getFaceCount();f++){
		        	  faceCount[f][0]=(short) mesh.getFaceVertexIndex(f, 0);
		        	  faceCount[f][1]=(short) mesh.getFaceVertexIndex(f, 1);
		        	  faceCount[f][2]=(short) mesh.getFaceVertexIndex(f, 2);
		          }
		          System.out.println("Old nTriangles:\t"+out.movi.nTriangles);
		          out.movi.setnewSize(faceCount.length, inject);
		          out.movi.render();
		          System.out.println("New nTriangles:\t"+out.movi.nTriangles);
		          for (int j = 0; j < faceCount.length; j++)
		            {
		              out.movi.setnewIndex(j, faceCount[j]);
		              
		            }
		          
		          if(useuv==true){
		          UVMapping uvmap=(UVMapping) info.getObject().getTextureMapping();      
		          Vec2 uv[]=uvmap.findTextureCoordinates(mesh);
		          out.motv.setnewSize(out.movt.nVertices, inject);
		          for(int n=0;n<out.movt.nVertices;n++){
		        	  if(n<=uv.length){
		        	  float[] txcoord={(float) uv[n].x,(float) uv[n].y};
		        	  out.motv.setnewTexVertex(n, txcoord);}
		        	  else{
		        		  float[] txcoord={0.0f,0.0f};
		        		  out.motv.setnewTexVertex(n, txcoord);
		        	  }
		          }}
		          if(inject==false){
		          out.moba.setnewSize(1, false);
		          out.movt.render();
		          out.movi.render();
		          out.moba.entrys[0]=new MOBA_Entry((short)0,(short)out.movt.nVertices,0,out.movi.nTriangles);
		          out.mobn.setnewSize(1, false);
		          out.mobn.entrys[0]=new MOBN_Entry(out.movi.nTriangles, 1);
		          }*/
		          /* out.monr.setnewSize(norm.length);
		          out.monr.render();
		          for (int j = 0; j < norm.length; j++)
		            {
		        	  float[] tf = new float[3];
		              if (norm[j] == null){
		            	  tf[0]=1f;tf[1]=0f;tf[2]=0f;
		            	  out.monr.setNormalPos(j, tf);}
		              else
		                {
		                  Vec3 v = trans.timesDirection(norm[j]);
		                  tf[0]=(float) v.x;tf[1]=(float) v.y;tf[2]=(float)v.z;
		                  out.monr.setnewNormalPos(j, tf);
		                }
		            }*/
		          		          
		          numVert += vert.length;
		          numNorm += norm.length;
		        }
		    save(out);
	  }
	  
	  public static int save(m2 out) {
			//TODO: catch failures and so on
			
				
				out.render();
				fileLoader.saveBuffer(out.buff , f.getAbsolutePath());
				return 1;
			
		}
}