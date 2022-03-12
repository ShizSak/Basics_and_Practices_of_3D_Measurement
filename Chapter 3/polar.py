#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import matplotlib
#matplotlib.use('Qt5Agg')

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("./scan.csv", names=['theta', 'dist'])
df["theta"] = np.deg2rad(df["theta"])

fig = plt.figure(figsize=(8,6))
ax = fig.add_subplot(111, projection='polar')
ax.plot(df["theta"], df["dist"], color='black', linewidth=0.5)
ax.scatter(df["theta"], df["dist"], s=10, color='gray')
ax.set_ylim([0.0, 600.0])
ax.set_xlim([0.0, np.pi])
ax.set_xticks(np.arange(0.0, np.pi+0.1, np.pi/12))
fig.tight_layout()
plt.show()
