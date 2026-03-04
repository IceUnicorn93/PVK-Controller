---
layout: default
title: PVK Console Blog
---

# PVK Console – Project Blog

Welcome to the PVK Console project development blog!

Here you'll find the latest updates, progress reports, and technical details
about the development of the desktop replica of the PVKK planetary defense console.

## Recent Posts

<ul>
  {% for post in site.posts %}
    <li>
      <h3><a href="{{ post.url | relative_url }}">{{ post.title }}</a></h3>
      <p><small>{{ post.date | date: "%B %d, %Y" }}</small></p>
      <p>{{ post.excerpt }}</p>
    </li>
  {% endfor %}
</ul>