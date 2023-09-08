# Webserv Project - My Learning Journey

## Table of Contents
- [Introduction](#introduction)
- [My Journey](#my-journey)
  - [Project Overview](#project-overview)
  - [HTTP Fundamentals](#http-fundamentals)
  - [Server Implementation](#server-implementation)
    - [Non-Crashing Code](#non-crashing-code)
    - [Makefile Mastery](#makefile-mastery)
    - [C++ 98 Compatibility](#c-98-compatibility)
    - [Non-blocking I/O](#non-blocking-io)
    - [Error Handling](#error-handling)
    - [Port Handling](#port-handling)
  - [Configuration File](#configuration-file)
    - [Dynamic Configuration](#dynamic-configuration)
    - [Route Configuration](#route-configuration)
  - [HTTP Handling](#http-handling)
    - [Web Content Delivery](#web-content-delivery)
    - [HTTP Methods](#http-methods)
    - [Client Uploads](#client-uploads)
    - [Static Websites](#static-websites)
  - [Bonus Challenges](#bonus-challenges)
    - [Cookies and Sessions](#cookies-and-sessions)
    - [Multiple CGI Handling](#multiple-cgi-handling)
- [Key Takeaways](#key-takeaways)
- [Conclusion](#conclusion)

---

## Introduction

This README documents my journey and learnings from the Webserv project, where I created my own HTTP server in C++ 98. This project was an opportunity to delve into the intricacies of HTTP, build a functional web server, and gain a deeper understanding of web protocols.

## My Webserv Version

**Webserv Version:** 21

## My Journey

### Project Overview

The Webserv project challenged me to develop a custom HTTP server from scratch. Here's a brief recap of what I achieved and learned:

### HTTP Fundamentals

I started by gaining a solid grasp of the Hypertext Transfer Protocol (HTTP). Understanding its role as the backbone of the World Wide Web was crucial to building a functional server.

### Server Implementation

- **Non-Crashing Code:** I learned to write robust code that never crashes, even when encountering unexpected situations or running out of memory. This was a crucial aspect of the project's success.

- **Makefile Mastery:** I created a Makefile to compile my source files, ensuring that the build process was smooth and efficient.

- **C++ 98 Compatibility:** The project required adherence to the C++ 98 standard, and I made sure my code complied seamlessly with this standard.

- **Non-blocking I/O:** Implementing non-blocking I/O operations using `poll()` or equivalent was a significant challenge. This ensured that the server could handle multiple clients without blocking.

- **Error Handling:** I developed robust error-handling mechanisms, eliminating unexpected crashes or abrupt server terminations.

- **Port Handling:** My server was capable of listening on multiple ports, enhancing its flexibility.

### Configuration File

- **Dynamic Configuration:** I implemented a configuration file system that allowed for dynamic server configuration. This included defining ports, hosts, server names, and more.

- **Route Configuration:** The configuration file enabled me to set up routes with various rules, such as specifying accepted HTTP methods, handling HTTP redirections, defining directory locations, and more.

### HTTP Handling

- **Web Content Delivery:** My server could store, process, and deliver web content, including HTML documents, images, style sheets, and scripts.

- **HTTP Methods:** I implemented support for essential HTTP methods like GET, POST, and DELETE.

- **Client Uploads:** The server allowed clients to upload files, expanding its functionality.

- **Static Websites:** I ensured that the server could serve fully static websites, demonstrating its capability to handle real-world scenarios.

### Bonus Challenges

While not mandatory, I took on some additional challenges:

- **Cookies and Sessions:** I explored cookie and session management, enhancing the server's interactivity.

- **Multiple CGI Handling:** I extended the server to handle multiple CGI (Common Gateway Interface) requests.

## Key Takeaways

Through the Webserv project, I gained invaluable knowledge and skills:

- A deep understanding of HTTP and its role in web communication.
- Proficiency in C++ 98 and creating non-crashing code.
- Expertise in server configuration and dynamic route handling.
- The ability to handle essential HTTP methods and client uploads.
- Experience in serving static websites and implementing bonus features.
- Insights into cookie and session management.
- The capability to manage multiple CGI requests efficiently.

## Conclusion

The Webserv project has been an enlightening journey, equipping me with the skills and knowledge to create a functional HTTP server. I hope this README provides insights into the project's scope, challenges, and the valuable lessons I've learned along the way. Thank you for joining me on this learning adventure.

[Go to Introduction](#introduction)
[Go to Project Overview](#project-overview)
