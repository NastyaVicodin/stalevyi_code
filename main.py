import cv2
import imutils
import socket

server_ip = '192.168.1.2'  # Raspberry Pi IP address
server_port = 5555

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Opening video stream from the built-in or external camera
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()

    # Converting colors from BGR to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Defining the brightness area (adjust parameters if necessary)
    lower_bright = (0, 100, 100)
    upper_bright = (10, 255, 255)
    mask = cv2.inRange(hsv, lower_bright, upper_bright)

    # Finding contours in the selected area
    contours = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours = imutils.grab_contours(contours)

    if contours:
        # Finding the largest contour (bright spot)
        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)

        if M["m00"] != 0:
            # Determining the coordinates of the bright spot's center
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])

            # Displaying the center on the image
            cv2.circle(frame, (cx, cy), 10, (0, 255, 0), 2)

            # Sending coordinates to Raspberry Pi
            data = f"{cx},{cy}"
            s.sendto(data.encode(), (server_ip, server_port))

    # Displaying the image
    cv2.imshow("Frame", frame)

    # Exiting the program when 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Closing the video stream and window
cap.release()
cv2.destroyAllWindows()
