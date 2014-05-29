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
print('parsed_query', parsed_query.get_description());

let enquire = new Xapian.Enquire({
    'database': db
});
enquire.init(null);
enquire.set_query(parsed_query, parsed_query.get_length());

let matches = enquire.get_mset(0, 10);
print('matches_size', matches.get_size())

let iter = Xapian.MSetIterator.alloc().init(matches);
while (iter.next()) {
    print(iter.get_document())
}
