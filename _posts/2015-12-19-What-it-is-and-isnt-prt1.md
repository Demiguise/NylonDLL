---
published: true
layout: post
title:  "What it is and isn't - Part 1"
date:   2015-12-19 17:50:00
categories: 
---

As I spoke about in the last post, this project has a mission statement for what it will become which i think is important to expand upon. 

> "The objective/mission statement is to create a robust, straight forward DLL dedicated
> to handling fiber creation, execution and deletion"

That way I can discuss what the project will do for the user and what is expected from the user by the project. In a sense...

## What it isn't.

The first thing I'm not going to do for users is memory management for their data. Originally the NylonDLL was integrated into my simple raytracer, so all of the data was available to it. I tried to make some parts of it generic enough that it could be re-used but I feel as thought I failed in that. While they were 'Generic' they were also pointless and a bit bloated. It also meant a distinct coupling between the user (Raytracing application) and the (Then un-named) nylon library.

So, after thinking long about if I could provide that service to the user I've decided to take it off the table. Although I could in theory provide some level of memory management to them I also feel it'd expose way too much of the implementation to them. In addition, I don't like the idea of them using this library and wondering why their application has bloated in memory usage (When it turns out my library is broken/doing something they didn't expect). 

To aid this, the NylonDLL should offer a simple method of linking a job to some Void* memory block _they_ are in control of. The only memory this library will actually deal with internally is the fiber objects, the priority queue and some level of debugging data.

The second thing I'm not going to do is hide the implementation details for fibers in general. I had planned to explore some method of variadic templating in order to completely remove the fiber syntax from their codebase. However, I now realise just how dangerous that is. The user needs to see the syntax, understand that multithreading is in action and then be able to appropriately change their code to match the needs. If I were to, via some crazy macro-age, hide the syntax I would be deliberatly obsfuscating what is happening inside the code. Which is bad.

As a side note, I planned to have some macro that would take a variadic number of arguments and boil them down into a templated job data struct. This struct would have the exact number and type of arguments needed to fulfill this function, all with the right names. When it came for that particular function to run, the fiber would grab the data and then feed the data directly into the function itself. In turn the end-user would just write a normal function instead of the ones I'm currently using (An explanation for another post I think, more meaty technical stuff later)

To continue the second point, I'm ensuring that the user has to know the basics of fiber usage so that they in turn get to understand multithreading. They will need to know how to declare one, how fibers keep get a small bit of storage for themselves, their stack space, etc.

Both of these points help to define what the API should look like and how the user should interact within their codepaths. There are still many unanswered questions about how some parts should work that I'm going to go through in the next post. With more meaty technicalness I think.
