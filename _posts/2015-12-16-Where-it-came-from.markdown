---
layout: post
title:  "Where it came from"
date:   2015-12-16 22:15:00
categories: 
---

Nylon was born out of watching a talk at GDC from Naughty Dog's Tech lead. In it, he detailed various parts of their engine and how they continued their work from the PS3 job system into the next age.
The whole thing was centralised around the HD remastering of 'The Last of Us', bringing it up to 60 fps through the use of fibers. 

That sounded **incredibly** cool to me.

So much so that I wanted to try and make something with it that would be beneficial to my learning about new technologies. Thus I went ahead and tried to make a raycaster. 
Raycasting is simple to start off with and gets more complex the more you go down. It's also perfect for using fibres because of how the data and the process is laid out.
At the beginning, each individual pixel can be thought of as a single job that runs parallel to every other pixel that's currently running. You do a line-intersection test on your geometry and, if it hits something, you colour it.

After a couple days of initial tinkering I got something out of it, a very cool image of a couple of spheres (Or circles, if you want):

![Screenshot]({{ site.url }}/NylonDLL/assets/RaycasterSpheres.png)

I wanted to progress some more and push forward with getting the ray tracer to look nicer and nicer.
I was really quite excited until I kept hitting stalls and crashes with the multithreading. My naive implementation of a scheduler was holding me back a bunch. It wasn't atomic, didn't really anything without a lock on it.

It frustrated me and I wanted to fix it, so I thought to take it out of the raytracer for a bit to focus on it. A DLL. I've not made one before, not really thought that much about API design but I've decided to design something that's straight forward to use.
I'm hoping that it provides a new platform to test ideas on and learn from new mistakes.

Gotta have a learning objective too, I find that really helpful in focusing myself and reaching a goal. Once I've hit that objective, I'll decide on a new one and it'll keep going.

The objective/mission statement is to create a robust, straight forward DLL dedicated to handling fiber creation, execution and deletion. It should be able to handle running several fibers at once with each one pulling jobs off of an priority queue without crashing.
In addition, it should be able to have jobs start, birth children, be deleted mid job and subsequent deletion/orphaning of children. It'd also be nice for it to have a good solid debugging platform that users can look at graphically to get an idea of what they are dealing with.
How each job interacted with which, birthed which children, got stopped, started, yiedled and deleted.

I look forward to posting some more. :)