![codam logo](https://www.datocms-assets.com/4526/1560770259-logocodamblack-copy.svg)

## ❔ Basic Overview

`Philosophers` is a part of Codam (42 network) curriculum for 2020 cohort. The aim of this project is to learn the basics of `threading` a process, making threads, using `mutex`. 

The assignment is based on the [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) originally formulated in 1965.

Philosophers dine together at the same table. Each philosopher has their own plate at the table. There is a fork between each plate. The dish served is a kind of spaghetti which has to be eaten with two forks. Each philosopher can only alternately think and eat. Moreover, a philosopher can only eat their spaghetti when they have both a left and right fork. Thus two forks will only be available when their two nearest neighbors are thinking, not eating. After an individual philosopher finishes eating, they will put down both forks.


|  |  |  |
| --- | --- | --- |
| 🥄 | 👴 | 🥄 |
| 👴 | 🍝 | 👴 |
| 🥄 | 👴 | 🥄 |

## ⚙️ Options

| Name | Description | 
| --- | --- | 
| `number_of_philosophers` | the number of philosophers and also the number of forks | 
| `time_to_die` | if a philosopher doesn’t start eating ’time_to_die’ milliseconds after starting his last meal or the beginning of the simulation, it dies | 
| `time_to_eat` | the time it takes for a philosopher to eat. During that time he will need to keep the two forks | 
| `time_to_sleep` | the time the philosopher will spend sleeping | 
| `number_of_times_each_philosopher_must_eat` | argument is optional, if all philosophers eat at least `number_of_times_each_philosopher_must_eat` the simulation will stop. If not specified, the simulation will stop only at the death of a philosopher. | 

## 🖥️ Output
◦ `timestamp_in_ms` X has taken a fork  </br>
◦ `timestamp_in_ms` X is eating </br>
◦ `timestamp_in_ms` X is sleeping </br>
◦ `timestamp_in_ms` X is thinking </br>
◦ `timestamp_in_ms` X died </br>
