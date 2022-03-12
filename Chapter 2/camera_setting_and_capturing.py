# -*- coding: utf-8 -*-

'''
Set camera properties and capture images for calibration
written by Shizuo Sakamoto as example in the book, titled as "Basics of 3D measurement",
to be released by Ohmsha, Ltd.

- version 1.00 on 1st, April, 2022.
  developed on Mouse Computer H5-CML with Ubuntu 20.04LTS,

  Camera properties for video capture are at:
  https://docs.opencv.org/3.4/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d

'''


import sys
import os
import time

import cv2

os.environ["KIVY_NO_CONSOLELOG"] = "1"
from kivy.app import App
from kivy.core.window import Window
from kivy.lang import Builder
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.image import Image
from kivy.clock import Clock
from kivy.graphics.texture import Texture
from kivy.properties import StringProperty, ObjectProperty

Builder.load_string('''
<CameraSettings>:
    orientation: 'horizontal'
    BoxLayout:
        orientation: 'vertical'
        size_hint_x: 0.8
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_SHARPNESS'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_sharpness
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_AUTOFOCUS'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_autofocus
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_FOCUS'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_focus
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_AUTO_EXPOSURE'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_auto_exposure
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_EXPOSURE'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_exposure
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_GAIN'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_gain
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_IRIS'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_iris
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_GAMMA'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_gamma
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_AUTO_WB'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_auto_wb
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_WB_TEMPERATURE'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_wb_temperature
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
        BoxLayout:
            orientation: 'horizontal'
            Label:
                text: 'CAP_PROP_BUFFERSIZE'
                size_hint_y: None
                height: '32dp'
            TextInput:
                id: textinput_buffersize
                input_filter: 'float'
                text: '0.0'
                multiline: False
                size_hint_y: None
                height: '32dp'
#        Button:
#            text: 'get camera properties'
#            size_hint_y: None
#            height: '48dp'
#            on_press: root.get()
        Button:
            text: 'set camera properties'
            size_hint_y: None
            height: '48dp'
            on_press: root.set()
        Label:
            id: main_message
            text: 'message'
            size_hint_y: None
            height: '48dp'
    BoxLayout:
        orientation: 'vertical'
        Image:
            id: image
            size: (640, 480)
        ToggleButton:
            text: 'Play'
            on_press: root.play()
            size_hint_y: None
            height: '48dp'
        Button:
            text: 'Capture'
            size_hint_y: None
            height: '48dp'
            on_press: root.capture()
        ToggleButton:
            text: 'Display overflow pixels'
            on_press: root.display_overflows()
            size_hint_y: None
            height: '48dp'
        Button:
            text: 'Exit'
            size_hint_y: None
            height: '48dp'
            on_press: root.exit()
''')

class CameraSettings(BoxLayout):
    def __init__(self, **kwargs):
        super(CameraSettings, self).__init__(**kwargs)
        self.on_play = False
        self.on_display_overflows = False
        self.textinput_sharpness = self.ids['textinput_sharpness']
        self.textinput_autofocus = self.ids['textinput_autofocus']
        self.textinput_focus = self.ids['textinput_focus']
        self.textinput_auto_exposure = self.ids['textinput_auto_exposure']
        self.textinput_exposure = self.ids['textinput_exposure']
        self.textinput_gain = self.ids['textinput_gain']
        self.textinput_iris = self.ids['textinput_iris']
        self.textinput_gamma = self.ids['textinput_gamma']
        self.textinput_auto_wb = self.ids['textinput_auto_wb']
        self.textinput_wb_temperature = self.ids['textinput_wb_temperature']
        self.textinput_buffersize = self.ids['textinput_buffersize']
        self.main_message = self.ids['main_message']

    def get(self):
        if self.on_play:
            self.textinput_sharpness.text = str(self.cap.get(cv2.CAP_PROP_SHARPNESS))
            self.textinput_autofocus.text = str(self.cap.get(cv2.CAP_PROP_AUTOFOCUS))
            self.textinput_focus.text = str(self.cap.get(cv2.CAP_PROP_FOCUS))
            self.textinput_auto_exposure.text = str(self.cap.get(cv2.CAP_PROP_AUTO_EXPOSURE))
            self.textinput_exposure.text = str(self.cap.get(cv2.CAP_PROP_EXPOSURE))
            self.textinput_gain.text = str(self.cap.get(cv2.CAP_PROP_GAIN))
            self.textinput_iris.text = str(self.cap.get(cv2.CAP_PROP_IRIS))
            self.textinput_gamma.text = str(self.cap.get(cv2.CAP_PROP_GAMMA))
            self.textinput_auto_wb.text = str(self.cap.get(cv2.CAP_PROP_AUTO_WB))
            self.textinput_wb_temperature.text = str(self.cap.get(cv2.CAP_PROP_WB_TEMPERATURE))
            self.textinput_buffersize.text = str(self.cap.get(cv2.CAP_PROP_BUFFERSIZE))
            self.main_message.text = "camera_settings.get(): OK"
        else:
            self.main_message.text = "camera_settings.get(): Camera open error"

    def set(self):
        if self.on_play:
            self.cap.set(cv2.CAP_PROP_SHARPNESS, float(self.textinput_sharpness.text))
            self.textinput_sharpness.text = str(self.cap.get(cv2.CAP_PROP_SHARPNESS))
            self.cap.set(cv2.CAP_PROP_AUTOFOCUS, float(self.textinput_autofocus.text))
            self.textinput_autofocus.text = str(self.cap.get(cv2.CAP_PROP_AUTOFOCUS))
            self.cap.set(cv2.CAP_PROP_FOCUS, float(self.textinput_focus.text))
            self.textinput_focus.text = str(self.cap.get(cv2.CAP_PROP_FOCUS))
            self.cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, float(self.textinput_auto_exposure.text))
            self.textinput_auto_exposure.text = str(self.cap.get(cv2.CAP_PROP_AUTO_EXPOSURE))
            self.cap.set(cv2.CAP_PROP_EXPOSURE, float(self.textinput_exposure.text))
            self.textinput_exposure.text = str(self.cap.get(cv2.CAP_PROP_EXPOSURE))
            self.cap.set(cv2.CAP_PROP_GAIN, float(self.textinput_gain.text))
            self.textinput_gain.text = str(self.cap.get(cv2.CAP_PROP_GAIN))
            self.cap.set(cv2.CAP_PROP_IRIS, float(self.textinput_iris.text))
            self.textinput_iris.text = str(self.cap.get(cv2.CAP_PROP_IRIS))
            self.cap.set(cv2.CAP_PROP_GAMMA, float(self.textinput_gamma.text))
            self.textinput_gamma.text = str(self.cap.get(cv2.CAP_PROP_GAMMA))
            self.cap.set(cv2.CAP_PROP_AUTO_WB, float(self.textinput_auto_wb.text))
            self.textinput_auto_wb.text = str(self.cap.get(cv2.CAP_PROP_AUTO_WB))
            self.cap.set(cv2.CAP_PROP_WB_TEMPERATURE, float(self.textinput_wb_temperature.text))
            self.textinput_wb_temperature.text = str(self.cap.get(cv2.CAP_PROP_WB_TEMPERATURE))
            self.cap.set(cv2.CAP_PROP_BUFFERSIZE, float(self.textinput_buffersize.text))
            self.textinput_buffersize.text = str(self.cap.get(cv2.CAP_PROP_BUFFERSIZE))
            self.main_message.text = "camera_settings.set(): OK"
        else:
            self.main_message.text = "camera_settings.set(): Camera open error"

    def play(self):
        self.on_play = not self.on_play
        if self.on_play:
            self.cap = cv2.VideoCapture(4)
            if self.cap.isOpened():
                Clock.schedule_interval(self.update, 1.0 / 30.0)
                self.get()
                self.main_message.text = "camera_settings.play(): OK"
            else:
                self.main_message.text = "camera_settings.play(): Camera open error"
        else:
            Clock.unschedule(self.update)
            self.cap.release()

    def update(self, timeduration):
        if self.cap.isOpened():
            ret, self.frame = self.cap.read()
            if ret:
                if self.on_display_overflows:
                    ret0, self.frame = cv2.threshold(self.frame, 250, 0, cv2.THRESH_TOZERO_INV)
                buf = cv2.flip(self.frame, 0).tostring()
                self.texture = Texture.create(size = (self.frame.shape[1], self.frame.shape[0]), colorfmt = 'bgr')
                image = self.ids['image']
                self.texture.blit_buffer(buf, colorfmt = 'bgr', bufferfmt = 'ubyte')
                image.texture = self.texture
#                self.main_message.text = "camera_settings.update(): OK"
            else:
                self.main_message.text = "camera_settings.update(): Image read error"
        else:
            self.main_message.text = "camera_settings.update(): Camera open error"

    def display_overflows(self):
        self.on_display_overflows = not self.on_display_overflows

    def capture(self):
        image = self.ids['image']
        timestr = time.strftime("%Yxc%m%d_%H%M%S")
        image.export_to_png("IMG_{}.png".format(timestr))

    def exit(self):
        sys.exit(0)

class CameraSettingsApp(App):
    def build(self):
        Window.size = (1152, 624)
        return CameraSettings()


if __name__ == '__main__':
    CameraSettingsApp().run()

# end of program.
