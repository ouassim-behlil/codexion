# Codexion

*This project is handmade by me and may contain mistakes and errors, please review and fix any issue you find.*

## Introduction
Codexion is basically based on the Dining Philosophers problem, based on my searching, the problem statement may be defined as follow:

- K philosophers sit around a cirular table.
- Each philosopher alternates between thinking and eating.
- Between each pair of philosophers, there is one chopstick (which means K chopsticks in total).
- A philosopher must pick up two chopsticks (left and right) to eat.
- Only one philosopher can use a chopstick at time.

**The Challenge** is designing a synchronization mechanism so that philosophers can eat without causing **deadlock** (all waiting forever) or **starvation** (a philosopher never get a chance to eat).

