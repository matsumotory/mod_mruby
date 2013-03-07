$(document).ready(function() {
    var socket = io.connect('http://'+ server +':'+ port);
    var gdata = 0;
    var loadavg1  = 0;
    var loadavg5  = 0;
    var loadavg15 = 0;
    var total_access_prev = -1;
    var total_access_curr = -1;
    var total_kbyte_prev = -1;
    var total_kbyte_curr = -1;

    socket.on('connect', function() {
        console.log('connect');
    });

    socket.on('disconnect', function(){
        console.log('disconnect');
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
                    renderTo: 'container9',
                    type: 'spline',
                    marginRight: 10,
                    plotBackgroundColor: '#EEEEEE',
                    events: {
                        load: function() {
                            var load1  = this.series[0],
                                load5  = this.series[1],
                                load15 = this.series[2];
                            setInterval(function() {
                                var x = (new Date()).getTime(),
                                    y0 = loadavg1,
                                    y1 = loadavg5,
                                    y2 = loadavg15;
                                load1.addPoint([x, y0], true, true);
                                load5.addPoint([x, y1], false, true);
                                load15.addPoint([x, y2], false, true);
                            }, 1000);
                        }
                    }
                },
                title: {
                    text: 'Load Average [1 5 15]'
                },
                xAxis: {
                    type: 'datetime',
                    tickPixelInterval: 150
                },
                yAxis: {
                    title: {
                        text: 'loadavg'
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
                    name: 'load avg 1',
                    data: (function() {
                        var data = [],
                            time = (new Date()).getTime(),
                            i;

                        for (i = -19; i <= 0; i++) {
                            data.push({
                                x: time + i * 1000,
                                y: 0
                            });
                        }
                        return data;
                    })()
                }, {
                    name: 'load avg 5',
                    data: (function() {
                        var data = [],
                            time = (new Date()).getTime(),
                            i;

                        for (i = -19; i <= 0; i++) {
                            data.push({
                                x: time + i * 1000,
                                y: 0
                            });
                        }
                        return data;
                    })()
                }, {
                    name: 'load avg 15',
                    data: (function() {
                        var data = [], 
                            time = (new Date()).getTime(),
                            i;  
    
                        for (i = -19; i <= 0; i++) {
                            data.push({
                                x: time + i * 1000,
                                y: 0
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
                    renderTo: 'container8',
                    type: 'spline',
                    marginRight: 10,
                    plotBackgroundColor: '#EEEEEE',
                    events: {
                        load: function() {
                            var series = this.series[0];
                            setInterval(function() {

			                      if ((total_access_curr - total_access_prev)>0) {
                                var x = (new Date()).getTime(),
                                    y = (total_kbyte_curr -  total_kbyte_prev) * 1000 / (total_access_curr - total_access_prev);
			                      } else {
			                          var x = (new Date()).getTime(),
				                        y = 0;
			                      }
                                series.addPoint([x, y], true, true);
                            }, 1000);
                        }
                    }
                },
                title: {
                    text: 'Byte per Request'
                },
                xAxis: {
                    type: 'datetime',
                    tickPixelInterval: 150
                },
                yAxis: {
                    title: {
                        text: 'byte/req'
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
                    name: 'byte per reqeust',
                    color: '#00008B',
                    data: (function() {
                        var data = [],
                            time = (new Date()).getTime(),
                            i;
        
                        for (i = -19; i <= 0; i++) {
			                      if ((total_access_curr - total_access_prev) > 0) {
                                data.push({
                                    x: time + i * 1000,
                                    y: (total_kbyte_curr -  total_kbyte_prev) * 1000 / (total_access_curr - total_access_prev)
			                          });
			                      } else {
			                          data.push({
			                              x: time + i * 1000,
			                              y: 0
			                          });
			                      }
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
                    renderTo: 'container5',
                    type: 'spline',
                    marginRight: 10,
                    plotBackgroundColor: '#EEEEEE',
                    events: {
                        load: function() {
                            var series = this.series[0];
                            setInterval(function() {
                                var x = (new Date()).getTime(),
                                    y = (total_access_curr - total_access_prev) / 5;
                                series.addPoint([x, y], true, true);
                            }, 1000);
                        }
                    }
                },
                title: {
                    text: 'Request per Second'
                },
                xAxis: {
                    type: 'datetime',
                    tickPixelInterval: 150
                },
                yAxis: {
                    title: {
                        text: 'req/sec'
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
                    name: 'request per sec',
                    color: '#006D56',
                    data: (function() {
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
                    renderTo: 'container6',
                    type: 'spline',
                    marginRight: 10,
                    plotBackgroundColor: '#EEEEEE',
                    events: {
                        load: function() {
                            var series = this.series[0];
                            setInterval(function() {
                                var x = (new Date()).getTime(), 
                                    y = (total_kbyte_curr - total_kbyte_prev) * 1000 / 5;
                                series.addPoint([x, y], true, true);
                            }, 1000);
                        }
                    }
                },
                title: {
                    text: 'Byte per Second'
                },
                xAxis: {
                    type: 'datetime',
                    tickPixelInterval: 150
                },
                yAxis: {
                    title: {
                        text: 'byte/sec'
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
                    name: 'byte per sec',
                    color: '#DF3447',
                    data: (function() {
                        var data = [],
                            time = (new Date()).getTime(),
                            i;

                        for (i = -19; i <= 0; i++) {
                            data.push({
                                x: time + i * 1000,
                                y: (total_kbyte_curr - total_kbyte_prev) * 1000 / 5
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
        loadavg1  = val.loadavg[0];
        loadavg5  = val.loadavg[1];
        loadavg15 = val.loadavg[2];
	
	    if (total_access_prev == -1) {
	    	total_access_prev = val.total_access;
	    } else {
	    	total_access_prev = total_access_curr;
	    }
	    
	    total_access_curr = val.total_access;
            
        if (total_kbyte_prev == -1) {
            total_kbyte_prev = val.total_kbyte;
        } else {
            total_kbyte_prev = total_kbyte_curr;
        }

        total_kbyte_curr = val.total_kbyte;

	    var chart;
        $(document).ready(function() {
        chart = new Highcharts.Chart({
            chart: {
                renderTo: 'container7',
		            spacingRight: 180,
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: false
            },
            title: {
                text: 'Thread status for request'
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
                            return '<b>'+ this.point.name +'</b>: '+ Math.round(this.percentage) +' %';
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
                    //['SERVER_DEAD',           val.counter.SERVER_DEAD],
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
                text: 'Apache busy/idle Worker Chart'
            },
            xAxis: {
                categories: ['worker']
            },
            yAxis: {
                min: 0,
                title: {
                    text: 'the number of worker'
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
                text: 'Apache Total Access'
            },
            xAxis: {
                categories: ['total_access']
            },
            yAxis: {
                min: 0,
                title: {
                    text: 'the number of total access'
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
                name: 'the number of access',
                color: '#006D56',
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
                text: 'Apache Total Byte'
            },
            xAxis: {
                categories: ['total_byte']
            },
            yAxis: {
                min: 0,
                title: {
                    text: 'total byte'
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
                name: 'total_byte',
                color: '#DF3447',
                data: [val.total_kbyte * 1000]
            }]
        });
    });
    });
});
