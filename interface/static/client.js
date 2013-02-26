$(document).ready(function() {
    var socket = io.connect('http://'+ server +':'+ port);

    socket.on('connect', function() {
        console.log('connect');
    });

    socket.on('disconnect', function(){
        console.log('disconnect');
    });

    //socket.on('change', function(val) {
    //    console.log('change:' + val);
    //    $('#val').append(val);
    //    $('#container').append(val);
    //});

    socket.on('change', function(val) {
        console.log('change:' + val);
        $('#val').append(val);
        var chart;
        $(document).ready(function() {
            chart = new Highcharts.Chart({
                chart: {
                    renderTo: 'container',
                    type: 'bar'
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
