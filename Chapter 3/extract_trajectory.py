#!/usr/bin/env python3

'''
Show a trajectory of the 3D scanner as result of integrating
RGB-D data sequence.
written by Shizuo Sakamoto as example in the book, titled as
"Basics of 3D measurement", to be released by Ohmsha, Ltd.

- version 1.00 on 1st, April, 2022.
  developed on Mouse Computer H5-CML, Ubuntu 20.04LTS,
  

  The code of read trajectory.log files came from:
  http://www.open3d.org/docs/0.13.0/tutorial/pipelines/rgbd_integration.html

'''

import sys
import os
import numpy as np

class CameraPose:
    def __init__(self, meta, mat):
        self.metadata = meta
        self.pose = mat
    def __str__(self):
        return 'Metadata : ' + ' '.join(map(str, self.metadata)) + '\n' + \
            "Pose : " + "\n" + np.array_str(self.pose)

def read_trajectory(filename):
    traj = []
    with open(filename, 'r') as f:
        metastr = f.readline()
        while metastr:
            metadata = map(int, metastr.split())
            mat = np.zeros(shape = (4, 4))
            for i in range(4):
                matstr = f.readline()
                mat[i,:] = np.fromstring(matstr, dtype = float, sep = ' \t')
            traj.append(CameraPose(metadata, mat))
            metastr = f.readline()
    return traj


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("trajectory.log file is required.")
    elif not os.path.exists(sys.argv[1]):
        print(sys.argv[1], "doesn't exist.")
    else:
        traj = read_trajectory(sys.argv[1])
        for x in traj:
            p = x.pose.tolist()
            print(p[0][3], ",", p[1][3], ",", p[2][3], ",255,255,255")
# end of program
