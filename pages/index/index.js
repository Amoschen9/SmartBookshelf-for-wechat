
var app = getApp()
var util = require('../../utils/util.js');
Page({
  data: {
    ah: "00",//闹钟时
    am: "00",//闹钟分
    as:"00",//闹钟秒
    status: "OFF",
    status2:"ON",
    t1:"",
    h:"",//系统时间的时分秒
    m:"",
    s:"",
    n:"1",
    nn:"100",
    nnn:6,
    chenshuo:"关闭"
    
  },
  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  },
  input1:function(e){
    if (e.detail.value != "") {
      this.setData({
        n: e.detail.value,

      })
    }
  },
  send1:function(){
    this.data.status2 = ((this.data.status2 == "ON") ? "OFF" : "ON"),
      this.setData({
        status2: this.data.status2
      })
    wx.showToast({
      title: this.data.status2,
      duration: 1000
    })
  },
  
 send2:  function () {
   var that = this//防止指向错误
   
   var a = setInterval(function () {//令时钟进行刷新
     
     var time = util.formatTime(new Date()); // 调用函数时，传入new Date()参数，返回值是日期和时间
     that.setData({
       t1: time
     });
     var time0 = util.formatTime0(new Date());
     that.setData({
       h: time0
     });
     var time1 = util.formatTime1(new Date());
     that.setData({
       m: time1
     });
     var time2 = util.formatTime2(new Date());
     that.setData({
       s: time2
     });
     if (((that.data.h == that.data.ah) && (that.data.m == that.data.am)) && (that.data.s == that.data.as)) 
     {
       that.setData({
         status: "ON"
          
       })  
     } 
     if(that.data.nnn=="7")
     {
       clearInterval(a)
     }
    
     that.sendRequset(that.makeObj(that.data.status));//循环执行的代码 
   }, 1000) //循环时间 这里是1秒
   
  },
  send3:function(){
   this.setData({nnn:this.data.nnn==6?7:6 ,
   chenshuo:this.data.chenshuo=="开启"?"关闭":"开启"
   })
  },
  send: function () {
    this.data.status = ((this.data.status == "ON") ? "OFF" : "ON"),
      this.setData({
        status: this.data.status
      })
    wx.showToast({
      title: this.data.status,
      duration: 1000
    })
    this.sendRequset(this.makeObj(this.data.status));
    },
    
    
  
  change1: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮
    if (e.detail.value != "") {
      this.setData({
        ah: e.detail.value,

      })
    }},
    change2: function (e) {
      //当有输入时激活发送按钮，无输入则禁用按钮
      if (e.detail.value != "") {
        this.setData({
          am: e.detail.value,

        })
      }

    },
      change3: function (e) {
        //当有输入时激活发送按钮，无输入则禁用按钮
        if (e.detail.value != "") {
          this.setData({
            as: e.detail.value,

          })
        }},

  sendRequset: function (obj) {
    wx.request(obj);
  },
  makeObj: function ( sta,msg) {
    var obj = {
      url: "http://api.heclouds.com/devices/577386286/datapoints?type=3",

      header: {
        "Content-Type": "application/json",
        "api-key": "JwW7wq1EF0y7=w5DsonfqLQwq=E=",
        //"Host": "api.heclouds.com"
      },
      method: "post",
      data: {
        //msuci id,playing status,playing precent
        
        "status": ((this.data.status == "ON") ? this.data.n: 0),
        "status2": (((this.data.status2 == "ON")&&(this.data.status=="ON"))?this.data.nn:99)
      },
      success: function (res) {
        if (msg != "") {
          wx.showToast({
            title: msg,
            duration: 2000
          })
          //console.log(i);
        }
      }
    }
    return obj;

  }  
       



})
