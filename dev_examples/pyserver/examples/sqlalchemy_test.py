#!/bin/env python

from sqlalchemy import *
    
def main():



    # Connecting
    engine =create_engine("mysql+mysqldb://root:tm@192.168.17.153/test",pool_size=20, max_overflow=0,pool_timeout=8)

    from sqlalchemy.ext.declarative import declarative_base
    Base = declarative_base()


    # Declare a Mapping
    from sqlalchemy import Column, Integer, String
    class T(Base):
        __tablename__ = 't'

        id = Column(Integer, primary_key=True)
        name = Column(String)
        
        def __repr__(self):
            return "<t(name='%s')>" % (self.name)


            
    from sqlalchemy.orm import sessionmaker
    Session = sessionmaker(bind=engine)
    session = Session()
    t1 = T(name='jc')
    print 't1=',t1
    #session.add(t1)
    #session.commit()


    t2 = session.query(T).filter_by(name='jc').first() 
    print 't2=',t2

if __name__=='__main__':
    main()

