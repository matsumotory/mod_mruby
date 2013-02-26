$(document).ready(function() {
    var socket = io.connect('http://'+ server +':'+ port);

    socket.on('connect', function() {
        console.log('connect');
    });

    socket.on('disconnect', function(){
        console.log('disconnect');
    });

    var gdata = 0;
    var total_access_prev = -1;
    var total_access_curr = -1;

    //socket.on('change', function(val) {
    //    console.log('change:' + val);
    //    $('#val').append(val);
    //    $('#container').append(val);
    //});


    $(function () {
        $(document).ready(function() {
            Highcharts.setOptions({
                global: {
                    useUTC: false
                }
            });
        
            var chart;
            chart = new Highcharts.Chart({
                chart: {
                    renderTo: 'container5',
                    type: 'spline',
                    marginRight: 10,
                    events: {
                        load: function() {
        
                            // set up the updating of the chart each second
                            var series = this.series[0];
                            setInterval(function() {
                                var x = (new Date()).getTime(), // current time
                                    //y = Math.random();
                                    y = (total_access_curr - total_access_prev) / 5;
                                series.addPoint([x, y], true, true);
                            }, 1000);
                        }
                    }
                },
                title: {
                    text: 'Access per second'
                },
                xAxis: {
                    type: 'datetime',
                    tickPixelInterval: 150
                },
                yAxis: {
                    title: {
                        text: 'Access/s'
                    },
                    plotLines: [{
                        value: 0,
                        width: 1,
                        color: '#808080'
                    }]
                },
                tooltip: {
                    formatter: function() {
                            return '<b>'+ this.series.name +'</b><br/>'+
                            Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) +'<br/>'+
                            Highcharts.numberFormat(this.y, 2);
                    }
                },
                legend: {
                    enabled: false
                },
                exporting: {
                    enabled: false
                },
                series: [{
                    name: 'Random data',
                    data: (function() {
                        // generate an array of random data
                        var data = [],
                            time = (new Date()).getTime(),
                            i;
        
                        for (i = -19; i <= 0; i++) {
                            data.push({
                                x: time + i * 1000,
                                y: (total_access_curr - total_access_prev) / 5
                            });
                        }
                        return data;
                    })()
                }]
            });
        });
    });


    $(function () {
        $(document).ready(function() {
            Highcharts.setOptions({
                global: {
                    useUTC: false
                }
            });
        
            var chart;
            chart = new Highcharts.Chart({
                chart: {
                    renderTo: 'container4',
                    type: 'spline',
                    marginRight: 10,
                    events: {
                        load: function() {
        
                            // set up the updating of the chart each second
                            var series = this.series[0];
                            setInterval(function() {
                                var x = (new Date()).getTime(), // current time
                                    //y = Math.random();
                                    y = gdata;
                                series.addPoint([x, y], true, true);
                            }, 1000);
                        }
                    }
                },
                title: {
                    text: 'Live random data'
                },
                xAxis: {
                    type: 'datetime',
                    tickPixelInterval: 150
                },
                yAxis: {
                    title: {
                        text: 'Value'
                    },
                    plotLines: [{
                        value: 0,
                        width: 1,
                        color: '#808080'
                    }]
                },
                tooltip: {
                    formatter: function() {
                            return '<b>'+ this.series.name +'</b><br/>'+
                            Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) +'<br/>'+
                            Highcharts.numberFormat(this.y, 2);
                    }
                },
                legend: {
                    enabled: false
                },
                exporting: {
                    enabled: false
                },
                series: [{
                    name: 'Random data',
                    data: (function() {
                        // generate an array of random data
                        var data = [],
                            time = (new Date()).getTime(),
                            i;
        
                        for (i = -19; i <= 0; i++) {
                            data.push({
                                x: time + i * 1000,
                                //y: Math.random()
                                y: gdata
                            });
                        }
                        return data;
                    })()
                }]
            });
        });
        
    });

    socket.on('change', function(val) {
        console.log('change:' + val);
        $('#val').append(val);
        gdata = val.idle_worker;
	
	if(total_access_prev == -1){
		total_access_prev = val.total_access;
	}
	else {
		total_access_prev = total_access_curr;
	}
	
	total_access_curr = val.total_access;
        
	var chart;
  $(document).ready(function() {
      chart = new Highcharts.Chart({
          chart: {
              renderTo: 'container7',
              plotBackgroundColor: null,
              plotBorderWidth: null,
              plotShadow: false
          },
          title: {
              text: 'thread status for request'
          },
          tooltip: {
            pointFormat: '{series.name}: <b>{point.percentage}%</b>',
            percentageDecimals: 1
          },
          plotOptions: {
              pie: {
                  allowPointSelect: true,
                  cursor: 'pointer',
                  dataLabels: {
                      enabled: true,
                      color: '#000000',
                      connectorColor: '#000000',
                      formatter: function() {
                          return '<b>'+ this.point.name +'</b>: '+ this.percentage +' %';
                      }
                  }
              }
          },
          series: [{
              type: 'pie',
              name: 'thread status',
              data: [
                  ['SERVER_GRACEFUL',       val.counter.SERVER_GRACEFUL],
                  ['SERVER_BUSY_READ',      val.counter.SERVER_BUSY_READ],
                  ['SERVER_IDLE_KILL',      val.counter.SERVER_IDLE_KILL],
                  ['SERVER_DEAD',           val.counter.SERVER_DEAD],
                  ['SERVER_BUSY_LOG',       val.counter.SERVER_BUSY_LOG],
                  ['SERVER_BUSY_DNS',       val.counter.SERVER_BUSY_DNS],
                  ['SERVER_CLOSING',        val.counter.SERVER_CLOSING],
                  ['SERVER_STARTING',       val.counter.SERVER_STARTING],
                  ['SERVER_BUSY_WRITE',     val.counter.SERVER_BUSY_WRITE],
                  ['SERVER_BUSY_KEEPALIVE', val.counter.SERVER_BUSY_KEEPALIVE],
                  ['SERVER_READY',          val.counter.SERVER_READY]
              ]
          }]
      });
  });

        $(document).ready(function() {
            chart = new Highcharts.Chart({
                chart: {
                    renderTo: 'container',
                    type: 'bar',
                    animation: false
                },
                title: {
                    text: 'Apache Status Chart'
                },
                xAxis: {
                    categories: ['worker']
                },
                yAxis: {
                    min: 0,
                    title: {
                        text: 'Total Apache Status consumption'
                    }
                },
                legend: {
                    backgroundColor: '#FFFFFF',
                    reversed: true
                },
                tooltip: {
                    formatter: function() {
                        return ''+
                            this.series.name +': '+ this.y +'';
                    }
                },
                plotOptions: {
                    series: {
                        stacking: 'normal'
                    }
                }, 
                    series: [{
                    name: 'idle',
                    data: [val.idle_worker]
                }, {
                    name: 'busy',
                    data: [val.busy_worker]
                }]
            });
        });

        $(document).ready(function() {
            chart = new Highcharts.Chart({
                chart: {
                    renderTo: 'container2',
                    type: 'bar'
                },
                title: {
                    text: 'Apache Status Chart'
                },
                xAxis: {
                    categories: ['total_access']
                },
                yAxis: {
                    min: 0,
                    title: {
                        text: 'Total Apache Status consumption'
                    }
                },
                legend: {
                    backgroundColor: '#FFFFFF',
                    reversed: true
                },
                tooltip: {
                    formatter: function() {
                        return ''+
                            this.series.name +': '+ this.y +'';
                    }
                },
                plotOptions: {
                    series: {
                        stacking: 'normal'
                    }
                }, 
                    series: [{
                    name: 'counter',
                    data: [val.total_access]
                }]
            });
        });

        $(document).ready(function() {
            chart = new Highcharts.Chart({
                chart: {
                    renderTo: 'container3',
                    type: 'bar'
                },
                title: {
                    text: 'Apache Status Chart'
                },
                xAxis: {
                    categories: ['total_kbyte']
                },
                yAxis: {
                    min: 0,
                    title: {
                        text: 'Total Apache Status consumption'
                    }
                },
                legend: {
                    backgroundColor: '#FFFFFF',
                    reversed: true
                },
                tooltip: {
                    formatter: function() {
                        return ''+
                            this.series.name +': '+ this.y +'';
                    }
                },
                plotOptions: {
                    series: {
                        stacking: 'normal'
                    }
                }, 
                    series: [{
                    name: 'total_kbyte',
                    data: [val.total_kbyte]
                }]
            });
        });
    });
});
