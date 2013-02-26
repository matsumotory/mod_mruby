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
                    text: 'Stacked bar chart'
                },
                xAxis: {
                    categories: ['Apples']
                },
                yAxis: {
                    min: 0,
                    title: {
                        text: 'Total fruit consumption'
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
                    name: 'John',
                    data: [val]
                }]
            });
        });
    });

});
