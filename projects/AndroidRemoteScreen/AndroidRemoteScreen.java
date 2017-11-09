/*
 * Advanced Computer Security. Project 4
 * =====================================
 * Android Remote Screen.
 * ======================
 *
 * Madhurima Roy: SBUID: 108997204
 * Supratik Singha: SBUID: 108997392
 *
 * Please refer to README for Instructions
 */

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.SocketChannel;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.PaletteData;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import com.android.ddmlib.RawImage;

public class AndroidRemoteScreen {
    private Label mImageLabel;
    private RawImage mRawImage;
    private boolean mRotateImage = false;

    private final static String ADB_HOST = "127.0.0.1";
    private final static int ADB_PORT = 5037;
    private final static int WAIT_TIME = 5;  // ms
    private static int max_x = 0, max_y = 0;
    private static int edown_x = 0;
    private static int edown_y = 0;
    private static int eup_x = 0;
    private static int eup_y = 0;
    private final static int swipe_factor = 50; // 50 pixels
    private final static int scale_factor = 2;
    private final static boolean trace_debug = false;

    private void open() throws IOException {
	if(trace_debug) System.out.println("debug:trace open");
        Display.setAppName("Android Projector");
        Display display = new Display();
	Shell shell = new Shell(display);
        shell.setText("Device Screen");
        createContents(shell);
	shell.open();

	/*
	mImageLabel.addMouseMoveListener(new MouseMoveListener() {

		public void mouseMove(MouseEvent e) {
		}
		});

	mImageLabel.addMouseTrackListener(new MouseTrackListener() {

		public void mouseEnter(MouseEvent arg0) {
		}

		public void mouseExit(MouseEvent arg0) {
		}

		public void mouseHover(MouseEvent arg0) {
		}
		});
	 */

	mImageLabel.addMouseListener(new MouseListener() {
		public void mouseDown(MouseEvent e) {
		    if(trace_debug) System.out.println("debug:trace open");

		    System.out.println("debug: Mouse Button Down at:" + e.x * scale_factor + " " + e.y * scale_factor);
		    edown_x = e.x;
		    edown_y = e.y;
		}

		public void mouseUp(MouseEvent e) {
                    boolean swipe = false;
		    if(trace_debug) System.out.println("debug:trace open");

		    System.out.println("debug: Mouse Button Up at:" + e.x * scale_factor + " " + e.y * scale_factor);
		    eup_x = e.x;
		    eup_y = e.y;

		    if(Math.abs(eup_x - edown_x) > swipe_factor || Math.abs(eup_y - edown_y) > swipe_factor)
		    	swipe = true;

		    SocketChannel adbShellChannel = null;
		    try {

			adbShellChannel = connectAdbDevice();
			if (adbShellChannel != null) {
			    if(swipe) {
			    	System.out.println(      "debug: shell:input swipe " + edown_x * scale_factor + " "
					                                             + edown_y * scale_factor + " "
					                                             + eup_x * scale_factor + " "
					                                             + eup_y * scale_factor + " ");
			    	sendAdbRequest(adbShellChannel, "shell:input swipe " + edown_x * scale_factor + " "
					                                             + edown_y * scale_factor + " "
					                                             + eup_x * scale_factor + " "
					                                             + eup_y * scale_factor + " ");
			    } else {
			    	System.out.println(      "debug: shell:input tap " + e.x * scale_factor + " " + e.y * scale_factor);
			    	sendAdbRequest(adbShellChannel, "shell:input tap " + e.x * scale_factor + " " + e.y * scale_factor);
			    }
			    if (!checkAdbResponse(adbShellChannel)) {
				System.out.println("debug: input keyevent returned null");
			    } else {
				System.out.println("debug: input keyevent OK");
			    }
			}

			try {
			    Thread.sleep(1000);
			} catch (Exception en)  {
			    System.out.println(en);
			}

			adbShellChannel.close();

		    } catch (IOException ex) {
			System.out.println("debug: EXCEPTION while trying to send command");
		    }
		}

		public void mouseDoubleClick(MouseEvent e) {
		    if(trace_debug) System.out.println("debug:trace mouseDoubleClick");

		}
		});

        SocketChannel adbChannel = null;
        try {
            while (!shell.isDisposed()) {
                if (!display.readAndDispatch()) {
                    adbChannel = connectAdbDevice();
                    if (adbChannel == null)
                        break;

		    System.out.println("debug: fetching device image");
                    if (startFramebufferRequest(adbChannel)) {
                        getFramebufferData(adbChannel);
                        updateDeviceImage(shell, mRotateImage ? mRawImage.getRotated() : mRawImage);
                    }

                    adbChannel.close();
		}

            }
        } finally {
            if (adbChannel != null)
                adbChannel.close();
            display.dispose();
        }
    }

    private void sendButtonCommand(String commandString) {

	SocketChannel adbButtonChannel = null;
	try {
	    adbButtonChannel = connectAdbDevice();
	    if (adbButtonChannel != null) {
		System.out.println("debug: " + commandString);
		sendAdbRequest(adbButtonChannel, commandString);
		if (!checkAdbResponse(adbButtonChannel)) {
		    System.out.println("debug: input keyevent returned null");
		} else {
		    System.out.println("debug: input keyevent OK");
		}
	    }

	    try {
		Thread.sleep(1000);
	    } catch (Exception en)  {
		System.out.println(en);
	    }

	    adbButtonChannel.close();

	} catch (IOException ex) {
	    System.out.println("debug: EXCEPTION while trying to send command");
	}
    }

    private void createContents(Shell shell) {
	if(trace_debug) System.out.println("debug:trace createContents");
        Menu menuBar = new Menu(shell, SWT.BAR);

        MenuItem viewItem = new MenuItem(menuBar, SWT.CASCADE);
        viewItem.setText("&Buttons");
        Menu viewMenu = new Menu(menuBar);
        viewItem.setMenu(viewMenu);

        final MenuItem powerItem = new MenuItem(viewMenu, SWT.PUSH);
        final MenuItem volUpItem = new MenuItem(viewMenu, SWT.PUSH);
        final MenuItem volDownItem = new MenuItem(viewMenu, SWT.PUSH);

        powerItem.setText("Power");
        powerItem.addSelectionListener(new SelectionAdapter() {
		public void widgetSelected(SelectionEvent e) {
			sendButtonCommand("shell:input keyevent KEYCODE_POWER");
		}
        });

        volUpItem.setText("Volume Up");
        volUpItem.addSelectionListener(new SelectionAdapter() {
            public void widgetSelected(SelectionEvent e) {
			sendButtonCommand("shell:input keyevent KEYCODE_VOLUME_UP");
            }
        });

        volDownItem.setText("Volume Down");
        volDownItem.addSelectionListener(new SelectionAdapter() {
            public void widgetSelected(SelectionEvent e) {
			sendButtonCommand("shell:input keyevent KEYCODE_VOLUME_DOWN");
            }
        });

        shell.setMenuBar(menuBar);
        shell.setLayout(new RowLayout());
        mImageLabel = new Label(shell, SWT.BORDER);
        mImageLabel.pack();
        shell.pack();
    }

    private SocketChannel connectAdbDevice() throws IOException {
	if(trace_debug) System.out.println("debug:trace connectAdbDevice");
        InetAddress hostAddr;
        try {
            hostAddr = InetAddress.getByName(ADB_HOST);
        } catch (UnknownHostException e) {
            return null;
        }

        InetSocketAddress socketAddr = new InetSocketAddress(hostAddr, ADB_PORT);
        SocketChannel adbChannel = SocketChannel.open(socketAddr);
        adbChannel.configureBlocking(true);

        // Select first USB device.
        sendAdbRequest(adbChannel, "host:transport-usb");
        if (!checkAdbResponse(adbChannel))
            return null;

        return adbChannel;
    }

    private boolean startFramebufferRequest(SocketChannel adbChannel) throws IOException {
	if(trace_debug) System.out.println("debug:trace startFramebufferRequest");
        // Request device framebuffer.
        sendAdbRequest(adbChannel, "framebuffer:");
        if (checkAdbResponse(adbChannel)) {
            getFramebufferHeader(adbChannel);
            return true;
        }

        return false;
    }

    private void getFramebufferHeader(SocketChannel adbChannel) throws IOException {
	if(trace_debug) System.out.println("debug:trace getFramebufferHeader");
        // Get protocol version.
        ByteBuffer buf = ByteBuffer.wrap(new byte[4]);
        readAdbChannel(adbChannel, buf);
        buf.rewind();
        buf.order(ByteOrder.LITTLE_ENDIAN);
        int version = buf.getInt();
        int headerSize = RawImage.getHeaderSize(version);

        // Get header.
        buf = ByteBuffer.wrap(new byte[headerSize * 4]);
        readAdbChannel(adbChannel, buf);
        buf.rewind();
        buf.order(ByteOrder.LITTLE_ENDIAN);
        mRawImage = new RawImage();
        mRawImage.readHeader(version, buf);
    }
    
    private void getFramebufferData(SocketChannel adbChannel) throws IOException {
	if(trace_debug) System.out.println("debug:trace getFramebufferData");
        // Send nudge.
        byte[] nudge = { 0 };
        ByteBuffer buf = ByteBuffer.wrap(nudge);
        writeAdbChannel(adbChannel, buf);

        // Receive framebuffer data.
        byte[] data = new byte[mRawImage.size];
        buf = ByteBuffer.wrap(data);
        readAdbChannel(adbChannel, buf);
        mRawImage.data = data;
    }

    private void sendAdbRequest(SocketChannel adbChannel, String request) throws IOException {
	if(trace_debug) System.out.println("debug:trace sendAdbRequest");
        String requestStr = String.format("%04X%s", request.length(), request);
        ByteBuffer buf = ByteBuffer.wrap(requestStr.getBytes());
        writeAdbChannel(adbChannel, buf);
    }

    private boolean checkAdbResponse(SocketChannel adbChannel) throws IOException {
	if(trace_debug) System.out.println("debug:trace checkAdbResponse");
        ByteBuffer buf = ByteBuffer.wrap(new byte[4]);
        readAdbChannel(adbChannel, buf);
        return buf.array()[0] == (byte)'O' && buf.array()[3] == (byte)'Y';
    }

    private void writeAdbChannel(SocketChannel adbChannel, ByteBuffer buf) throws IOException {
	if(trace_debug) System.out.println("debug:trace writeAdbChannel");
        while (buf.position() != buf.limit()) {
            int count = adbChannel.write(buf);
            if (count < 0) {
                throw new IOException("EOF");
            } else if (count == 0) {
                try {
                    Thread.sleep(WAIT_TIME);
                } catch (InterruptedException e) {
                }
            }
        }
    }

    private void readAdbChannel(SocketChannel adbChannel, ByteBuffer buf) throws IOException {
	if(trace_debug) System.out.println("debug:trace readAdbChannel");
        while (buf.position() != buf.limit()) {
            int count = adbChannel.read(buf);
            if (count < 0) {
                throw new IOException("EOF");
            } else if (count == 0) {
                try {
                    Thread.sleep(WAIT_TIME);
                } catch (InterruptedException e) {
                }
            }
        }
    }

    private void updateDeviceImage(Shell shell, RawImage rawImage) {
	if(trace_debug) System.out.println("debug:trace updateDeviceImage");
        PaletteData paletteData = new PaletteData(
                rawImage.getRedMask(),
                rawImage.getGreenMask(),
                rawImage.getBlueMask());
        ImageData imageData = new ImageData(
                rawImage.width,
                rawImage.height,
                rawImage.bpp,
                paletteData,
                1,
                rawImage.data);

	max_x = rawImage.width/scale_factor;
	max_y = rawImage.height/scale_factor;
	System.out.println("debug: Image: " + rawImage.width + "x" + rawImage.height + " scaled to " + max_x + "x" + max_y);

	imageData = imageData.scaledTo(max_x, max_y);
        Image image = new Image(shell.getDisplay(), imageData);
        mImageLabel.setImage(image);
        mImageLabel.pack();
        shell.pack();
    }

    public static void main(String[] args) {
	System.out.println("debug: Starting main");
        AndroidRemoteScreen androidRemoteScreen = new AndroidRemoteScreen();
        try {
            androidRemoteScreen.open();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
