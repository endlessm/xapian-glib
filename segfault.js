const Xapian = imports.gi.Xapian;

const query_string = "bah humbug";
const QUERY_PARSER_FLAGS = Xapian.QueryParserFeature.DEFAULT | Xapian.QueryParserFeature.WILDCARD;
const path = 'testdb';

let db = new Xapian.Database({
    'path': path
}); 
db.init(null);

let qp = new Xapian.QueryParser({
    'database': db,
});
let parsed_query = qp.parse_query(query_string, QUERY_PARSER_FLAGS);

let enquire = new Xapian.Enquire({
    'database': db
});
enquire.init(null);
enquire.set_query(parsed_query, parsed_query.length);

let matches = enquire.get_mset(0, 10);
print('size', matches.get_size()) // bogus value, like 858993458

let iter = new Xapian.MSetIterator();
iter.init(matches);

print(iter.get_document()) // segfault
