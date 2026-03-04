---
layout: default
title: PVK Console Blog
---

# 🚀 PVK Console – Project Blog

<div style="background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); padding: 30px; border-radius: 10px; color: white; margin-bottom: 40px;">
  <h2 style="margin-top: 0; color: white;">Welcome to the PVK Console Development Blog!</h2>
  <p style="font-size: 1.1em; line-height: 1.6;">Follow our journey as we build an open-source desktop replica of the iconic PVKK planetary defense console. Get the latest updates, technical insights, and join our growing community!</p>
</div>

---

## 📝 Latest Updates

<div style="margin-top: 30px;">
  {% for post in site.posts %}
    <article style="background: #f8f9fa; padding: 25px; margin-bottom: 30px; border-radius: 8px; border-left: 4px solid #667eea; box-shadow: 0 2px 4px rgba(0,0,0,0.1);">
      <h3 style="margin-top: 0;">
        <a href="{{ post.url | relative_url }}" style="color: #667eea; text-decoration: none;">{{ post.title }}</a>
      </h3>
      <p style="color: #6c757d; font-size: 0.9em; margin-bottom: 15px;">
        📅 {{ post.date | date: "%B %d, %Y" }}
      </p>
      <div style="color: #495057;">
        {{ post.excerpt }}
      </div>
      <a href="{{ post.url | relative_url }}" style="display: inline-block; margin-top: 10px; color: #667eea; text-decoration: none; font-weight: bold;">Read more →</a>
    </article>
  {% endfor %}
</div>

---

## 🔧 Project Components

| Component | Description | Status |
|-----------|-------------|--------|
| 🔌 Hardware | PCB design and electronics | In Development |
| 💾 Firmware | Microcontroller code | Planning |
| 💻 PC Software | Desktop application | In Development |
| 📚 Documentation | Guides and tutorials | Ongoing |

---

<div style="background: #e7f3ff; padding: 20px; border-radius: 8px; border-left: 4px solid #0066cc;">
  <h3 style="margin-top: 0;">🤝 Get Involved</h3>
  <p>This is a community-driven project and we welcome contributors! Whether you're into hardware design, firmware development, or software engineering – there's a place for you here.</p>
  <p><strong>Check out our <a href="https://github.com/IceUnicorn93/pvk-console">GitHub Repository</a> to get started!</strong></p>
</div>