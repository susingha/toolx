import boto3
import csv

def lambda_handler(event, context):
    region='ap-south-1'
    try: 
        # get a handle on s3
        session = boto3.Session(region_name=region)
        s3 = session.resource('s3')
        dyndb = boto3.client('dynamodb', region_name=region)
        bucket = s3.Bucket('trips-csv-database-store') 
        obj = bucket.Object(key='Trips.csv') 
        # get the object
        response = obj.get()
        # read the contents of the file
        lines = response['Body'].read().decode('utf-8').splitlines()
 
        firstrecord=True
        csv_reader = csv.reader(lines, delimiter=',', quotechar='"')
        for row in csv_reader:
            if (firstrecord):
                firstrecord=False
                continue

            _id          = row[0]
            _code        = row[1]
            _lat         = row[2]
            _lon         = row[3]
            _latlon      = row[4]
            _place       = row[5]
            _state       = row[6]
            _image       = row[7]
            _months      = row[8]
            _category    = row[9]
            _mindays     = row[10]
            _maxdays     = row[11]
            _description = row[12]

            response = dyndb.put_item(
                TableName='Trips-o3r6fcfzh5dytmorch7ztpq66u-dev',
                Item={
                # 'S' for type String, 'N' for Number.
                'id'         : {'S':str(_id)},
                'code'       : {'S':str(_code)},
                'lat'        : {'N':float(_lat)},
                'lon'        : {'N':float(_lon)},
                'latlon'     : {'S':str(_latlon)},
                'place'      : {'S':str(_place)},
                'state'      : {'S':str(_state)},
                'image'      : {'S':str(_image)},
                'months'     : {'S':str(_months)},
                'category'   : {'S':str(_category)},
                'mindays'    : {'N':int(_mindays)},
                'maxdays'    : {'N':int(_maxdays)},
                'description': {'S':str(_description)},                
                }
            )
        result = 'Put succeeded:'
    except Exception as err:
        result = format(err)
    return {
        'body': result
    }