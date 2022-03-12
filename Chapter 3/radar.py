#!/usr/bin/env python
# -*- coding: utf-8 -*-
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.clock import Clock
from kivy.lang import Builder
from kivy.garden.matplotlib.backend_kivyagg import FigureCanvasKivyAgg
import numpy as np
import matplotlib.pyplot as plt
import socket

kv_def = '''
<RootWidget>:
    orientation: 'vertical'
    Label:
        text: 'simple radar'
        size_hint_y: 0.1
    GraphView:
<GraphView>:
'''

class GraphView(BoxLayout):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.theta = [ int(x) for x in np.linspace(0,179,180)]
        self.dist = [ 0 for x in self.theta]

        self.fig, self.ax = plt.subplots(subplot_kw={'projection': 'polar'})
        self.line, = self.ax.plot(self.theta, self.dist, color='green')
        self.ax.set_ylim([0.0, 600.0])
        self.ax.set_xlim([0.0, np.pi])
        self.ax.set_xticks(np.arange(0.0, np.pi+0.1, np.pi/12))
        self.fig.tight_layout()
        widget = FigureCanvasKivyAgg(self.fig)
        self.add_widget(widget)

        Clock.schedule_interval(self.update_view, 0.01)

    def update_view(self, *args, **kwargs):
        try:
            data = s.recvfrom(1024)
            if len(data) > 0:
                sep = data[0].decode().split(",")
                self.dist[int(sep[0])] = int(sep[1])
                self.line.set_data(np.deg2rad(self.theta), self.dist)
                self.ax.relim()
                self.ax.autoscale_view()
                self.fig.canvas.draw()
                self.fig.canvas.flush_events()
        except:
            pass

class RootWidget(BoxLayout):
    """Empty"""

class GraphApp(App):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title = 'Simple Radar App'
    def build(self):
        return RootWidget()

Builder.load_string(kv_def)
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("0.0.0.0", 9999))
s.setblocking(False)
app = GraphApp()
app.run()