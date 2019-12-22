const formatTime = date => {
  const year = date.getFullYear()
  const month = date.getMonth() + 1
  const day = date.getDate()
  const hour = date.getHours()
  const minute = date.getMinutes()
  const second = date.getSeconds()

  return [year, month, day].map(formatNumber).join('/') + ' ' + [hour, minute, second].map(formatNumber).join(':')
}

const formatTime0 = date0 => {
  const year = date0.getFullYear()
  const month = date0.getMonth() + 1
  const day = date0.getDate()
  const hour = date0.getHours()
  const minute = date0.getMinutes()
  const second = date0.getSeconds()

  return hour
}
const formatTime1 = date1 => {
  const year = date1.getFullYear()
  const month = date1.getMonth() + 1
  const day = date1.getDate()
  const hour = date1.getHours()
  const minute = date1.getMinutes()
  const second = date1.getSeconds()

  return minute
}

const formatTime2 = date2 => {
  const year = date2.getFullYear()
  const month = date2.getMonth() + 1
  const day = date2.getDate()
  const hour = date2.getHours()
  const minute = date2.getMinutes()
  const second = date2.getSeconds()

  return second
}

const formatNumber = n => {
  n = n.toString()
  return n[1] ? n : '0' + n
}

module.exports = {
  formatTime: formatTime,
  formatTime0: formatTime0,
  formatTime1: formatTime1,
  formatTime2: formatTime2
}
